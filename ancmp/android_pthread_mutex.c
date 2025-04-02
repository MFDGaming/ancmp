#include <errno.h>
#include <stdlib.h>
#include "android_pthread_mutex.h"
#include "android_atomic.h"
#include "android_futex.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/syscall.h>
#endif
#include "android_errno.h"

void android_normal_lock(android_pthread_mutex_t *mutex, int shared) {
    const int unlocked           = shared | ANDROID_MUTEX_STATE_BITS_UNLOCKED;
    const int locked_uncontended = shared | ANDROID_MUTEX_STATE_BITS_LOCKED_UNCONTENDED;

    if (android_atomic_cmpxchg(unlocked, locked_uncontended, &mutex->value) != 0) {
        const int locked_contended = shared | ANDROID_MUTEX_STATE_BITS_LOCKED_CONTENDED;

        while (android_atomic_swap(locked_contended, &mutex->value) != unlocked) {
            android_futex_wait_ex(&mutex->value, shared, locked_contended, 0);
        }
    }
    ANDROID_MEMBAR_FULL();
}

void android_normal_unlock(android_pthread_mutex_t *mutex, int shared) {
    ANDROID_MEMBAR_FULL();
    if (android_atomic_dec(&mutex->value) != (shared|ANDROID_MUTEX_STATE_BITS_LOCKED_UNCONTENDED)) {
        mutex->value = shared;
        android_futex_wake_ex(&mutex->value, shared, 1);
    }
}

int android_recursive_increment(android_pthread_mutex_t *mutex, int mvalue, int mtype) {
    if (mtype == ANDROID_MUTEX_TYPE_BITS_ERRORCHECK) {
        return ANDROID_EDEADLK;
    }
    if (ANDROID_MUTEX_COUNTER_BITS_WILL_OVERFLOW(mvalue)) {
        return ANDROID_EAGAIN;
    }
    for (;;) {
        int newval = mvalue + ANDROID_MUTEX_COUNTER_BITS_ONE;
        if (android_atomic_cmpxchg(mvalue, newval, &mutex->value) == 0) {
            return 0;
        }
        mvalue = mutex->value;
    }
}

int android_pthread_mutex_init(android_pthread_mutex_t *mutex, const android_pthread_mutexattr_t *attr) {
    int value = 0;
    if (mutex == NULL) {
        return ANDROID_EINVAL;
    }
    if (attr == NULL) {
        mutex->value = ANDROID_MUTEX_TYPE_BITS_NORMAL;
        return 0;
    }
    if ((*attr & ANDROID_MUTEXATTR_SHARED_MASK) != 0) {
        value |= ANDROID_MUTEX_SHARED_MASK;
    }
    switch (*attr & ANDROID_MUTEXATTR_TYPE_MASK) {
    case ANDROID_PTHREAD_MUTEX_NORMAL:
        value |= ANDROID_MUTEX_TYPE_BITS_NORMAL;
        break;
    case ANDROID_PTHREAD_MUTEX_RECURSIVE:
        value |= ANDROID_MUTEX_TYPE_BITS_RECURSIVE;
        break;
    case ANDROID_PTHREAD_MUTEX_ERRORCHECK:
        value |= ANDROID_MUTEX_TYPE_BITS_ERRORCHECK;
        break;
    default:
        return ANDROID_EINVAL;
    }
    mutex->value = value;
    return 0;
}

int android_pthread_mutex_destroy(android_pthread_mutex_t *mutex) {
    int ret;
    ret = android_pthread_mutex_trylock(mutex);
    if (ret != 0) {
        return ret;
    }
    mutex->value = 0xdead10cc;
    return 0;
}

int android_pthread_mutex_trylock(android_pthread_mutex_t *mutex) {
    int mvalue, mtype, tid, oldv, shared;
    if (mutex == NULL) {
        return ANDROID_EINVAL;
    }
    mvalue = mutex->value;
    mtype  = (mvalue & ANDROID_MUTEX_TYPE_MASK);
    shared = (mvalue & ANDROID_MUTEX_SHARED_MASK);
    if (mtype == ANDROID_MUTEX_TYPE_BITS_NORMAL) {
        if (android_atomic_cmpxchg(shared|ANDROID_MUTEX_STATE_BITS_UNLOCKED,
                             shared|ANDROID_MUTEX_STATE_BITS_LOCKED_UNCONTENDED,
                             &mutex->value) == 0) {
            ANDROID_MEMBAR_FULL();
            return 0;
        }
        return ANDROID_EBUSY;
    }
#ifdef _WIN32
    tid = GetCurrentThreadId();
#else
    tid = syscall(SYS_gettid);
#endif
    if (tid == ANDROID_MUTEX_OWNER_FROM_BITS(mvalue)) {
        return android_recursive_increment(mutex, mvalue, mtype);
    }
    mtype |= shared | ANDROID_MUTEX_STATE_BITS_UNLOCKED;
    mvalue = ANDROID_MUTEX_OWNER_TO_BITS(tid) | mtype | ANDROID_MUTEX_STATE_BITS_LOCKED_UNCONTENDED;
    if (android_atomic_cmpxchg(mtype, mvalue, &mutex->value) == 0) {
        ANDROID_MEMBAR_FULL();
        return 0;
    }
    return ANDROID_EBUSY;
}

int android_pthread_mutex_lock(android_pthread_mutex_t *mutex) {
    int mvalue, mtype, tid, new_lock_type, shared;
    if (mutex == NULL) {
        return ANDROID_EINVAL;
    }
    mvalue = mutex->value;
    mtype = (mvalue & ANDROID_MUTEX_TYPE_MASK);
    shared = (mvalue & ANDROID_MUTEX_SHARED_MASK);
    if (mtype == ANDROID_MUTEX_TYPE_BITS_NORMAL) {
        android_normal_lock(mutex, shared);
        return 0;
    }
#ifdef _WIN32
    tid = GetCurrentThreadId();
#else
    tid = syscall(SYS_gettid);
#endif
    if (tid == ANDROID_MUTEX_OWNER_FROM_BITS(mvalue)) {
        return android_recursive_increment(mutex, mvalue, mtype);
    }
    mtype |= shared;
    if (mvalue == mtype) {
        int newval = ANDROID_MUTEX_OWNER_TO_BITS(tid) | mtype | ANDROID_MUTEX_STATE_BITS_LOCKED_UNCONTENDED;
        if (android_atomic_cmpxchg(mvalue, newval, &mutex->value) == 0) {
            ANDROID_MEMBAR_FULL();
            return 0;
        }
        mvalue = mutex->value;
    }
    for (;;) {
        int newval;
        if (mvalue == mtype) {
            newval = ANDROID_MUTEX_OWNER_TO_BITS(tid) | mtype | ANDROID_MUTEX_STATE_BITS_LOCKED_CONTENDED;
            if (android_atomic_cmpxchg(mvalue, newval, &mutex->value) != 0) {
                mvalue = mutex->value;
                continue;
            }
            ANDROID_MEMBAR_FULL();
            return 0;
        }
        if (ANDROID_MUTEX_STATE_BITS_IS_LOCKED_UNCONTENDED(mvalue)) {
            newval = ANDROID_MUTEX_STATE_BITS_FLIP_CONTENTION(mvalue);
            if (android_atomic_cmpxchg(mvalue, newval, &mutex->value) != 0) {
                mvalue = mutex->value;
                continue;
            }
            mvalue = newval;
        }
        android_futex_wait_ex(&mutex->value, shared, mvalue, NULL);
        mvalue = mutex->value;
    }
}

int android_pthread_mutex_unlock(android_pthread_mutex_t *mutex) {
    int mvalue, mtype, tid, oldv, shared;

    if (mutex == NULL) {
        return ANDROID_EINVAL;
    }
    mvalue = mutex->value;
    mtype  = (mvalue & ANDROID_MUTEX_TYPE_MASK);
    shared = (mvalue & ANDROID_MUTEX_SHARED_MASK);
    if (mtype == ANDROID_MUTEX_TYPE_BITS_NORMAL) {
        android_normal_unlock(mutex, shared);
        return 0;
    }
#ifdef _WIN32
    tid = GetCurrentThreadId();
#else
    tid = syscall(SYS_gettid);
#endif
    if (tid != ANDROID_MUTEX_OWNER_FROM_BITS(mvalue)) {
        return ANDROID_EPERM;
    }

    if (!ANDROID_MUTEX_COUNTER_BITS_IS_ZERO(mvalue)) {
        for (;;) {
            int newval = mvalue - ANDROID_MUTEX_COUNTER_BITS_ONE;
            if (android_atomic_cmpxchg(mvalue, newval, &mutex->value) == 0) {
                return 0;
            }
            mvalue = mutex->value;
        }
    }
    ANDROID_MEMBAR_FULL();
    mvalue = android_atomic_swap(mtype | shared | ANDROID_MUTEX_STATE_BITS_UNLOCKED, &mutex->value);

    if (ANDROID_MUTEX_STATE_BITS_IS_LOCKED_CONTENDED(mvalue)) {
        android_futex_wake_ex(&mutex->value, shared, 1);
    }
    return 0;
}

int android_pthread_mutexattr_init(android_pthread_mutexattr_t *attr) {
    if (attr) {
        *attr = ANDROID_PTHREAD_MUTEX_DEFAULT;
        return 0;
    } else {
        return ANDROID_EINVAL;
    }
}

int android_pthread_mutexattr_destroy(android_pthread_mutexattr_t *attr) {
    if (attr) {
        *attr = -1;
        return 0;
    } else {
        return ANDROID_EINVAL;
    }
}

int android_pthread_mutexattr_gettype(const android_pthread_mutexattr_t *attr, int *type) {
    if (attr) {
        int  atype = (*attr & ANDROID_MUTEXATTR_TYPE_MASK);
         if (atype >= ANDROID_PTHREAD_MUTEX_NORMAL &&
             atype <= ANDROID_PTHREAD_MUTEX_ERRORCHECK) {
            *type = atype;
            return 0;
        }
    }
    return ANDROID_EINVAL;
}

int android_pthread_mutexattr_settype(android_pthread_mutexattr_t *attr, int type) {
    if (attr && type >= ANDROID_PTHREAD_MUTEX_NORMAL &&
                type <= ANDROID_PTHREAD_MUTEX_ERRORCHECK ) {
        *attr = (*attr & ~ANDROID_MUTEXATTR_TYPE_MASK) | type;
        return 0;
    }
    return ANDROID_EINVAL;
}

int android_pthread_mutexattr_getpshared(const android_pthread_mutexattr_t *attr, int *pshared) {
    if (!attr || !pshared)
        return ANDROID_EINVAL;
    *pshared = (*attr & ANDROID_MUTEXATTR_SHARED_MASK) ? ANDROID_PTHREAD_PROCESS_SHARED
                                               : ANDROID_PTHREAD_PROCESS_PRIVATE;
    return 0;
}

int android_pthread_mutexattr_setpshared(android_pthread_mutexattr_t *attr, int pshared) {
    if (!attr)
        return ANDROID_EINVAL;
    switch (pshared) {
    case ANDROID_PTHREAD_PROCESS_PRIVATE:
        *attr &= ~ANDROID_MUTEXATTR_SHARED_MASK;
        return 0;
    case ANDROID_PTHREAD_PROCESS_SHARED:
        *attr |= ANDROID_MUTEXATTR_SHARED_MASK;
        return 0;
    }
    return ANDROID_EINVAL;
}
