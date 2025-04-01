#include "android_pthread_cond.h"
#include "android_pthread_mutex.h"
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "android_futex.h"
#include "android_atomic.h"
#include "posix_funcs.h"
#include "android_errno.h"

int android_timespec_to_absolute(android_timespec_t *ts, const android_timespec_t *abstime, android_clockid_t clock) {
    android_clock_gettime(clock, ts);
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

int android_pthread_cond_pulse(android_pthread_cond_t *cond, int counter) {
    long flags;
    if (cond == NULL) {
        return ANDROID_EINVAL;
    }
    flags = (cond->value & ~ANDROID_COND_COUNTER_MASK);
    for (;;) {
        long oldval = cond->value;
        long newval = ((oldval - ANDROID_COND_COUNTER_INCREMENT) & ANDROID_COND_COUNTER_MASK)
                      | flags;
        if (android_atomic_cmpxchg(oldval, newval, &cond->value) == 0) {
            break;
        }
    }
    ANDROID_MEMBAR_FULL();
    android_futex_wake_ex(&cond->value, ANDROID_COND_IS_SHARED(cond), counter);
    return 0;
}

int android_pthread_cond_timedwait_relative(android_pthread_cond_t *cond, android_pthread_mutex_t * mutex, const android_timespec_t *reltime) {
    int status;
    int oldvalue = cond->value;
    android_pthread_mutex_unlock(mutex);
    status = android_futex_wait_ex(&cond->value, ANDROID_COND_IS_SHARED(cond), oldvalue, reltime);
    android_pthread_mutex_lock(mutex);
    if (status == (-ANDROID_ETIMEDOUT)) {
        return ANDROID_ETIMEDOUT;
    }
    return 0;
}

int android_pthread_cond_timedwait2(android_pthread_cond_t *cond, android_pthread_mutex_t * mutex, const android_timespec_t *abstime, android_clockid_t clock) {
    android_timespec_t ts;
    android_timespec_t * tsp;
    if (abstime != NULL) {
        if (android_timespec_to_absolute(&ts, abstime, clock) < 0) {
            return ANDROID_ETIMEDOUT;
        }
        tsp = &ts;
    } else {
        tsp = NULL;
    }
    return android_pthread_cond_timedwait_relative(cond, mutex, tsp);
}

int android_pthread_cond_init(android_pthread_cond_t *cond, const android_pthread_condattr_t *attr) {
    if (cond == NULL) {
        return ANDROID_EINVAL;
    }
    cond->value = 0;
    if (attr != NULL && *attr == ANDROID_PTHREAD_PROCESS_SHARED) {
        cond->value |= ANDROID_COND_SHARED_MASK;
    }
    return 0;
}

int android_pthread_cond_destroy(android_pthread_cond_t *cond) {
    if (cond == NULL) {
        return ANDROID_EINVAL;
    }
    cond->value = 0xdeadc04d;
    return 0;
}

int android_pthread_cond_broadcast(android_pthread_cond_t *cond) {
    return android_pthread_cond_pulse(cond, INT_MAX);
}

int android_pthread_cond_signal(android_pthread_cond_t *cond) {
    return android_pthread_cond_pulse(cond, 1);
}

int android_pthread_cond_timedwait(android_pthread_cond_t *cond, android_pthread_mutex_t *mutex, const android_timespec_t *abstime) {
    return android_pthread_cond_timedwait2(cond, mutex, abstime, ANDROID_CLOCK_REALTIME);
}

int android_pthread_cond_wait(android_pthread_cond_t *cond, android_pthread_mutex_t *mutex) {
    return android_pthread_cond_timedwait(cond, mutex, NULL);
}

int android_pthread_condattr_init(android_pthread_condattr_t *attr) {
    if (attr == NULL) {
        return ANDROID_EINVAL;
    }
    *attr = ANDROID_PTHREAD_PROCESS_PRIVATE;
    return 0;
}

int android_pthread_condattr_destroy(android_pthread_condattr_t *attr) {
    if (attr == NULL) {
        return ANDROID_EINVAL;
    }
    *attr = 0xdeada11d;
    return 0;
}

int android_pthread_condattr_getpshared(const android_pthread_condattr_t *attr, int *pshared) {
    if (attr == NULL || pshared == NULL) {
        return ANDROID_EINVAL;
    }
    *pshared = *attr;
    return 0;
}

int android_pthread_condattr_setpshared(android_pthread_condattr_t *attr, int pshared) {
    if (attr == NULL) {
        return ANDROID_EINVAL;
    }
    if (pshared != ANDROID_PTHREAD_PROCESS_SHARED &&
        pshared != ANDROID_PTHREAD_PROCESS_PRIVATE) {
        return ANDROID_EINVAL;
    }
    *attr = pshared;
    return 0;
}