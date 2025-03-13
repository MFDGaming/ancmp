#include "android_pthread.h"
#include <pthread.h>
#include <errno.h>
#include "android_atomic.h"
#include "android_futex.h"
#include <limits.h>
#include <stdatomic.h>
#include "posix_funcs.h"

#define DEFAULT_STACKSIZE (1024 * 1024)
#define PAGE_SIZE 4096

static const android_pthread_attr_t gDefaultPthreadAttr = {
    .flags = 0,
    .stack_base = NULL,
    .stack_size = DEFAULT_STACKSIZE,
    .guard_size = PAGE_SIZE,
    .sched_policy = ANDROID_SCHED_NORMAL,
    .sched_priority = 0
};

int android_pthread_attr_init(android_pthread_attr_t * attr) {
    if (attr == NULL) {
        return EINVAL;
    }
    *((void **)attr) = malloc(sizeof(pthread_attr_t));
    return pthread_attr_init(*((void **)attr));
}

int android_pthread_attr_destroy(android_pthread_attr_t * attr) {
    if (attr == NULL) {
        return EINVAL;
    }
    int ret = pthread_attr_destroy(*((void **)attr));
    free(*((void **)attr));
    *((void **)attr) = NULL;
    return ret;
}

int android_pthread_attr_setdetachstate(android_pthread_attr_t * attr, int state) {
    if (attr == NULL) {
        return EINVAL;
    }
    if (state == ANDROID_PTHREAD_CREATE_DETACHED) {
        if (*((void **)attr) == NULL) {
            *((void **)attr) = malloc(sizeof(pthread_attr_t));
        }
        pthread_attr_setdetachstate(*((void **)attr), PTHREAD_CREATE_DETACHED);
    } else if (state == ANDROID_PTHREAD_CREATE_JOINABLE) {
        if (*((void **)attr) == NULL) {
            *((void **)attr) = malloc(sizeof(pthread_attr_t));
        }
        pthread_attr_setdetachstate(*((void **)attr), PTHREAD_CREATE_JOINABLE);
    } else {
        return EINVAL;
    }
    return 0;
}

int android_pthread_attr_setschedparam(android_pthread_attr_t * attr, struct android_sched_param const * param) {
    if (attr == NULL) {
        return EINVAL;
    }
    if (*((void **)attr) == NULL) {
        *((void **)attr) = malloc(sizeof(pthread_attr_t));
    }
    return pthread_attr_setschedparam(*((void **)attr), (struct sched_param *)param);
}

int android_pthread_attr_setstacksize(android_pthread_attr_t * attr, size_t stack_size) {
    if (attr == NULL) {
        return EINVAL;
    }
    if (*((void **)attr) == NULL) {
        *((void **)attr) = malloc(sizeof(pthread_attr_t));
    }
    return pthread_attr_setstacksize(*((void **)attr), stack_size);
}

static int android_pthread_cond_pulse(android_pthread_cond_t *cond, int counter) {
    long flags;

    if (cond == NULL)
        return EINVAL;

    flags = (cond->value & ~ANDROID_COND_COUNTER_MASK);
    for (;;) {
        long oldval = cond->value;
        long newval = ((oldval - ANDROID_COND_COUNTER_INCREMENT) & ANDROID_COND_COUNTER_MASK)
                      | flags;
        if (android_atomic_cmpxchg(oldval, newval, &cond->value) == 0)
            break;
    }

    android_futex_wake_ex(&cond->value, ANDROID_COND_IS_SHARED(cond), counter);
    return 0;
}

int android_pthread_cond_broadcast(android_pthread_cond_t *cond) {
    return android_pthread_cond_pulse(cond, INT_MAX);
}

int android_pthread_cond_destroy(android_pthread_cond_t *cond) {
    if (cond == NULL)
        return EINVAL;

    cond->value = 0xdeadc04d;
    return 0;
}

int android_pthread_cond_init(android_pthread_cond_t *cond, const android_pthread_condattr_t *attr) {
    if (cond == NULL)
        return EINVAL;

    cond->value = 0;

    if (attr != NULL && *attr == ANDROID_PTHREAD_PROCESS_SHARED)
        cond->value |= ANDROID_COND_SHARED_MASK;

    return 0;
}

static __inline__ void android_normal_lock(android_pthread_mutex_t *mutex) {
    int  shared = mutex->value & ANDROID_MUTEX_SHARED_MASK;
    
    if (android_atomic_cmpxchg(shared|0, shared|1, &mutex->value) != 0) {
        while (android_atomic_swap(shared|2, &mutex->value) != (shared|0)) {
            android_futex_wait_ex(&mutex->value, shared, shared|2, 0);
        }
    }
    atomic_thread_fence(__ATOMIC_SEQ_CST);
}

static __inline__ void android_normal_unlock(android_pthread_mutex_t *mutex) {
    atomic_thread_fence(__ATOMIC_SEQ_CST);

    int  shared = mutex->value & ANDROID_MUTEX_SHARED_MASK;
    
    if (android_atomic_dec(&mutex->value) != (shared|1)) {
        mutex->value = shared;
        android_futex_wake_ex(&mutex->value, shared, 1);
    }
}

static android_pthread_mutex_t  __recursive_lock = ANDROID_PTHREAD_MUTEX_INITIALIZER;

static void android_recursive_lock(void) {
    android_normal_lock(&__recursive_lock);
}

static void android_recursive_unlock(void) {
    android_normal_unlock(&__recursive_lock);
}

#ifdef _WIN32
    #include <windows.h>
    #define get_kernel_thread_id() GetCurrentThreadId()
#else
    #include <unistd.h>
    #include <sys/syscall.h>
    #define get_kernel_thread_id() syscall(SYS_gettid)
#endif

int android_pthread_mutex_trylock(android_pthread_mutex_t *mutex) {
    int mtype, tid, oldv, shared;

    if (mutex == NULL)
        return EINVAL;

    mtype  = (mutex->value & ANDROID_MUTEX_TYPE_MASK);
    shared = (mutex->value & ANDROID_MUTEX_SHARED_MASK);

    if (mtype == ANDROID_MUTEX_TYPE_NORMAL) {
        if (android_atomic_cmpxchg(shared|0, shared|1, &mutex->value) == 0) {
            atomic_thread_fence(__ATOMIC_SEQ_CST);
            return 0;
        }

        return EBUSY;
    }

    tid = get_kernel_thread_id();
    if ( tid == ANDROID_MUTEX_OWNER(mutex) )
    {
        int counter;

        if (mtype == ANDROID_MUTEX_TYPE_ERRORCHECK) {
            return EDEADLK;
        }

        android_recursive_lock();
        oldv = mutex->value;
        counter = (oldv + (1 << ANDROID_MUTEX_COUNTER_SHIFT)) & ANDROID_MUTEX_COUNTER_MASK;
        mutex->value = (oldv & ~ANDROID_MUTEX_COUNTER_MASK) | counter;
        android_recursive_unlock();
        return 0;
    }

    mtype |= shared;

    android_recursive_lock();
    oldv = mutex->value;
    if (oldv == mtype)
        mutex->value = ((tid << 16) | mtype | 1);
    android_recursive_unlock();

    if (oldv != mtype)
        return EBUSY;

    return 0;
}

static int
__timespec_to_absolute(struct timespec*  ts, const struct timespec*  abstime, clockid_t  clock)
{
    clock_gettime(clock, ts);
    ts->tv_sec  = abstime->tv_sec - ts->tv_sec;
    ts->tv_nsec = abstime->tv_nsec - ts->tv_nsec;
    if (ts->tv_nsec < 0) {
        ts->tv_sec--;
        ts->tv_nsec += 1000000000;
    }
    if ((ts->tv_nsec < 0) || (ts->tv_sec < 0))
        return -1;

    return 0;
}

static int android_pthread_cond_timedwait_relative(android_pthread_cond_t *cond, android_pthread_mutex_t * mutex, const struct timespec *reltime) {
    int  status;
    int  oldvalue = cond->value;

    android_pthread_mutex_unlock(mutex);
    status = android_futex_wait_ex(&cond->value, ANDROID_COND_IS_SHARED(cond), oldvalue, reltime);
    android_pthread_mutex_lock(mutex);

    if (status == (-ETIMEDOUT)) return ETIMEDOUT;
        return 0;
}

int android_pthread_cond_timedwait(android_pthread_cond_t *cond, android_pthread_mutex_t * mutex, const struct timespec *abstime) {
    struct timespec ts;
    struct timespec * tsp;

    if (abstime != NULL) {
        if (__timespec_to_absolute(&ts, abstime, CLOCK_REALTIME) < 0)
            return ETIMEDOUT;
        tsp = &ts;
    } else {
        tsp = NULL;
    }

    return android_pthread_cond_timedwait_relative(cond, mutex, tsp);
}

int android_pthread_cond_wait(android_pthread_cond_t *cond, android_pthread_mutex_t *mutex) {
    return android_pthread_cond_timedwait(cond, mutex, NULL);
}
 
int android_pthread_create(android_pthread_t *thread, android_pthread_attr_t const * attr, void *(*start_routine)(void *), void * arg) {
    pthread_attr_t *real_attr = NULL;
    if (attr && *((void **)attr)) {
        real_attr = *((void **)attr);
    }
    return pthread_create((pthread_t *)thread, real_attr, start_routine, arg);
}

void *android_pthread_getspecific(android_pthread_key_t key) {
    pthread_key_t _key = (pthread_key_t)key;
    return pthread_getspecific(_key);
}
 
int android_pthread_join(android_pthread_t thid, void ** ret_val) {
    return pthread_join(*((pthread_t *)(&thid)), ret_val);
}
 
int android_pthread_key_create(android_pthread_key_t *key, void (*destructor_function)(void *)) {
    return pthread_key_create((pthread_key_t *)key, destructor_function);
}

int android_pthread_mutexattr_destroy(android_pthread_mutexattr_t *attr) {
    if (attr) {
        *attr = -1;
        return 0;
    } else {
        return EINVAL;
    }
}
 
int android_pthread_mutexattr_init(android_pthread_mutexattr_t *attr) {
    if (attr) {
        *attr = ANDROID_PTHREAD_MUTEX_DEFAULT;
        return 0;
    } else {
        return EINVAL;
    }
}

int android_pthread_mutex_destroy(android_pthread_mutex_t *mutex) {
    int ret;

    ret = android_pthread_mutex_trylock(mutex);
    if (ret != 0)
        return ret;

    mutex->value = 0xdead10cc;
    return 0;
}

int android_pthread_mutex_init(android_pthread_mutex_t *mutex, const android_pthread_mutexattr_t *attr) {
    int value = 0;

    if (mutex == NULL)
        return EINVAL;

    if (attr == NULL) {
        mutex->value = ANDROID_MUTEX_TYPE_NORMAL;
        return 0;
    }

    if ((*attr & ANDROID_MUTEXATTR_SHARED_MASK) != 0)
        value |= ANDROID_MUTEX_SHARED_MASK;

    switch (*attr & ANDROID_MUTEXATTR_TYPE_MASK) {
    case ANDROID_PTHREAD_MUTEX_NORMAL:
        value |= ANDROID_MUTEX_TYPE_NORMAL;
        break;
    case ANDROID_PTHREAD_MUTEX_RECURSIVE:
        value |= ANDROID_MUTEX_TYPE_RECURSIVE;
        break;
    case ANDROID_PTHREAD_MUTEX_ERRORCHECK:
        value |= ANDROID_MUTEX_TYPE_ERRORCHECK;
        break;
    default:
        return EINVAL;
    }

    mutex->value = value;
    return 0;
}

int android_pthread_mutex_lock(android_pthread_mutex_t *mutex) {
    int mtype, tid, new_lock_type, shared;

    if (mutex == NULL)
        return EINVAL;

    mtype = (mutex->value & ANDROID_MUTEX_TYPE_MASK);
    shared = (mutex->value & ANDROID_MUTEX_SHARED_MASK);

    if (mtype == ANDROID_MUTEX_TYPE_NORMAL) {
        android_normal_lock(mutex);
        return 0;
    }

    tid = get_kernel_thread_id();
    if (tid == ANDROID_MUTEX_OWNER(mutex))
    {
        int  oldv, counter;

        if (mtype == ANDROID_MUTEX_TYPE_ERRORCHECK) {
            return EDEADLK;
        }

        android_recursive_lock();
        oldv         = mutex->value;
        counter      = (oldv + (1 << ANDROID_MUTEX_COUNTER_SHIFT)) & ANDROID_MUTEX_COUNTER_MASK;
        mutex->value = (oldv & ~ANDROID_MUTEX_COUNTER_MASK) | counter;
        android_recursive_unlock();
        return 0;
    }

    new_lock_type = 1;

    mtype |= shared;

    for (;;) {
        int  oldv;

        android_recursive_lock();
        oldv = mutex->value;
        if (oldv == mtype) {
            mutex->value = ((tid << 16) | mtype | new_lock_type);
        } else if ((oldv & 3) == 1) {
            oldv ^= 3;
            mutex->value = oldv;
        }
        android_recursive_unlock();

        if (oldv == mtype)
            break;

        new_lock_type = 2;

        android_futex_wait_ex(&mutex->value, shared, oldv, NULL);
    }
    return 0;
}

int android_pthread_mutex_unlock(android_pthread_mutex_t *mutex) {
    int mtype, tid, oldv, shared;

    if (mutex == NULL)
        return EINVAL;

    mtype  = (mutex->value & ANDROID_MUTEX_TYPE_MASK);
    shared = (mutex->value & ANDROID_MUTEX_SHARED_MASK);

    /* Handle common case first */
    if (mtype == ANDROID_MUTEX_TYPE_NORMAL) {
        android_normal_unlock(mutex);
        return 0;
    }

    /* Do we already own this recursive or error-check mutex ? */
    tid = get_kernel_thread_id();
    if ( tid != ANDROID_MUTEX_OWNER(mutex) )
        return EPERM;

    /* We do, decrement counter or release the mutex if it is 0 */
    android_recursive_lock();
    oldv = mutex->value;
    if (oldv & ANDROID_MUTEX_COUNTER_MASK) {
        mutex->value = oldv - (1 << ANDROID_MUTEX_COUNTER_SHIFT);
        oldv = 0;
    } else {
        mutex->value = shared | mtype;
    }
    android_recursive_unlock();

    /* Wake one waiting thread, if any */
    if ((oldv & 3) == 2) {
        android_futex_wake_ex(&mutex->value, shared, 1);
    }
    return 0;
}

android_pthread_t android_pthread_self(void) {
    pthread_t t = pthread_self();
    return *((android_pthread_t *)(&t)); 
}

int android_pthread_setspecific(android_pthread_key_t key, const void *value) {
    return pthread_setspecific((pthread_key_t)key, value);
}

int android_pthread_key_delete(android_pthread_key_t key) {
    return pthread_key_delete((pthread_key_t)key);
}

int android_pthread_once(android_pthread_once_t *once_control, void (*init_routine)(void)) {
    return pthread_once((pthread_once_t *)once_control, init_routine);
}

int android_pthread_equal(android_pthread_t t1, android_pthread_t t2) {
    pthread_t _t1 = *(pthread_t *)&t1;
    pthread_t _t2 = *(pthread_t *)&t2;
    return pthread_equal(_t1, _t2);
}

int android_pthread_cond_signal(android_pthread_cond_t *cond) {
    return android_pthread_cond_pulse(cond, 1);
}

int android_pthread_detach(android_pthread_t thread) {
    return pthread_detach(*(pthread_t *)&thread);
}

int android_pthread_setname_np(android_pthread_t thread, const char *name) {
    return pthread_setname_np(*(pthread_t *)&thread, name);
}