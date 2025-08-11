/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "android_semaphore.h"
#include "android_futex.h"
#include "android_atomic.h"
#include <errno.h>
#include <limits.h>

#ifndef ETIMEDOUT
#define ETIMEDOUT 110
#endif
#ifndef EOVERFLOW
#define EOVERFLOW 75
#endif

/* In this implementation, a semaphore contains a
 * 31-bit signed value and a 1-bit 'shared' flag
 * (for process-sharing purpose).
 *
 * We use the value -1 to indicate contention on the
 * semaphore, 0 or more to indicate uncontended state,
 * any value lower than -2 is invalid at runtime.
 *
 * State diagram:
 *
 * post(1)  ==> 2
 * post(0)  ==> 1
 * post(-1) ==> 1, then wake all waiters
 *
 * wait(2)  ==> 1
 * wait(1)  ==> 0
 * wait(0)  ==> -1 then wait for a wake up + loop
 * wait(-1) ==> -1 then wait for a wake up + loop
 *
 */
/* Use the upper 31-bits for the counter, and the lower one
 * for the shared flag.
 */
#define ANDROID_SEMCOUNT_SHARED_MASK      0x00000001
#define ANDROID_SEMCOUNT_VALUE_MASK       0xfffffffe
#define ANDROID_SEMCOUNT_VALUE_SHIFT      1
/* Maximum unsigned value that can be stored in the semaphore.
 * One bit is used for the shared flag, another one for the
 * sign bit, leaving us with only 30 bits.
 */
#define ANDROID_SEM_MAX_VALUE             0x3fffffff
/* convert a value into the corresponding sem->count bit pattern */
#define ANDROID_SEMCOUNT_FROM_VALUE(val)    (((val) << ANDROID_SEMCOUNT_VALUE_SHIFT) & ANDROID_SEMCOUNT_VALUE_MASK)
/* convert a sem->count bit pattern into the corresponding signed value */
#define ANDROID_SEMCOUNT_TO_VALUE(sval)  ((int)(sval) >> ANDROID_SEMCOUNT_VALUE_SHIFT)
/* the value +1 as a sem->count bit-pattern. */
#define ANDROID_SEMCOUNT_ONE              ANDROID_SEMCOUNT_FROM_VALUE(1)
/* the value -1 as a sem->count bit-pattern. */
#define ANDROID_SEMCOUNT_MINUS_ONE        ANDROID_SEMCOUNT_FROM_VALUE(0xffffffff)
#define ANDROID_SEMCOUNT_DECREMENT(sval)    (((sval) - (1U << ANDROID_SEMCOUNT_VALUE_SHIFT)) & ANDROID_SEMCOUNT_VALUE_MASK)
#define ANDROID_SEMCOUNT_INCREMENT(sval)    (((sval) + (1U << ANDROID_SEMCOUNT_VALUE_SHIFT)) & ANDROID_SEMCOUNT_VALUE_MASK)
/* return the shared bitflag from a semaphore */
#define ANDROID_SEM_GET_SHARED(sem)       ((sem)->count & ANDROID_SEMCOUNT_SHARED_MASK)

int android_sem_init(android_sem_t *sem, int pshared, unsigned int value) {
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    /* ensure that 'value' can be stored in the semaphore */
    if (value > ANDROID_SEM_MAX_VALUE) {
        errno = EINVAL;
        return -1;
    }
    sem->count = ANDROID_SEMCOUNT_FROM_VALUE(value);
    if (pshared != 0)
        sem->count |= ANDROID_SEMCOUNT_SHARED_MASK;
    return 0;
}

int android_sem_destroy(android_sem_t *sem) {
    int count;
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    count = ANDROID_SEMCOUNT_TO_VALUE(sem->count);
    if (count < 0) {
        errno = EBUSY;
        return -1;
    }
    sem->count = 0;
    return 0;
}

android_sem_t *android_sem_open(const char *name, int oflag, ...) {
    name=name;
    oflag=oflag;
    errno = ENOSYS;
    return ANDROID_SEM_FAILED;
}

int android_sem_close(android_sem_t *sem) {
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    errno = ENOSYS;
    return -1;
}

int android_sem_unlink(const char *name) {
    errno = ENOSYS;
    return -1;
}

/* Decrement a semaphore's value atomically,
 * and return the old one. As a special case,
 * this returns immediately if the value is
 * negative (i.e. -1)
 */
static int __android_sem_dec(volatile unsigned int *pvalue) {
    unsigned int shared = (*pvalue & ANDROID_SEMCOUNT_SHARED_MASK);
    unsigned int old, new;
    int          ret;
    do {
        old = (*pvalue & ANDROID_SEMCOUNT_VALUE_MASK);
        ret = ANDROID_SEMCOUNT_TO_VALUE(old);
        if (ret < 0)
            break;
        new = ANDROID_SEMCOUNT_DECREMENT(old);
    }
    while (android_atomic_cmpxchg((int)(old|shared),
                            (int)(new|shared),
                            (volatile int *)pvalue) != 0);
    return ret;
}

/* Same as __android_sem_dec, but will not touch anything if the
 * value is already negative *or* 0. Returns the old value.
 */
static int __android_sem_trydec(volatile unsigned int *pvalue) {
    unsigned int shared = (*pvalue & ANDROID_SEMCOUNT_SHARED_MASK);
    unsigned int old, new;
    int          ret;
    do {
        old = (*pvalue & ANDROID_SEMCOUNT_VALUE_MASK);
        ret = ANDROID_SEMCOUNT_TO_VALUE(old);
        if (ret <= 0)
            break;
        new = ANDROID_SEMCOUNT_DECREMENT(old);
    }
    while (android_atomic_cmpxchg((int)(old|shared),
                            (int)(new|shared),
                            (volatile int *)pvalue) != 0);
    return ret;
}

/* "Increment" the value of a semaphore atomically and
 * return its old value. Note that this implements
 * the special case of "incrementing" any negative
 * value to +1 directly.
 *
 * NOTE: The value will _not_ wrap above SEM_VALUE_MAX
 */
static int __android_sem_inc(volatile unsigned int *pvalue) {
    unsigned int  shared = (*pvalue & ANDROID_SEMCOUNT_SHARED_MASK);
    unsigned int  old, new;
    int           ret;
    do {
        old = (*pvalue & ANDROID_SEMCOUNT_VALUE_MASK);
        ret = ANDROID_SEMCOUNT_TO_VALUE(old);
        /* Can't go higher than SEM_MAX_VALUE */
        if (ret == ANDROID_SEM_MAX_VALUE)
            break;
        /* If the counter is negative, go directly to +1,
         * otherwise just increment */
        if (ret < 0)
            new = ANDROID_SEMCOUNT_ONE;
        else
            new = ANDROID_SEMCOUNT_INCREMENT(old);
    }
    while ( android_atomic_cmpxchg((int)(old|shared),
                             (int)(new|shared),
                             (volatile int*)pvalue) != 0);
    return ret;
}

/* lock a semaphore */
int android_sem_wait(android_sem_t *sem) {
    unsigned shared;
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    shared = ANDROID_SEM_GET_SHARED(sem);
    for (;;) {
        if (__android_sem_dec(&sem->count) > 0)
            break;
        android_futex_wait_ex(&sem->count, shared, shared|ANDROID_SEMCOUNT_MINUS_ONE, NULL);
    }
    ANDROID_MEMBAR_FULL();
    return 0;
}

int android_sem_timedwait(android_sem_t *sem, const android_timespec_t *abs_timeout) {
    int  ret;
    unsigned int shared;
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    /* POSIX says we need to try to decrement the semaphore
     * before checking the timeout value. Note that if the
     * value is currently 0, __android_sem_trydec() does nothing.
     */
    if (__android_sem_trydec(&sem->count) > 0) {
        ANDROID_MEMBAR_FULL();
        return 0;
    }
    /* Check it as per Posix */
    if (abs_timeout == NULL    ||
        abs_timeout->tv_sec < 0 ||
        abs_timeout->tv_nsec < 0 ||
        abs_timeout->tv_nsec >= 1000000000)
    {
        errno = EINVAL;
        return -1;
    }
    shared = ANDROID_SEM_GET_SHARED(sem);
    for (;;) {
        android_timespec_t ts;
        int             ret;
        /* Posix mandates CLOCK_REALTIME here */
        android_clock_gettime( ANDROID_CLOCK_REALTIME, &ts );
        ts.tv_sec  = abs_timeout->tv_sec - ts.tv_sec;
        ts.tv_nsec = abs_timeout->tv_nsec - ts.tv_nsec;
        if (ts.tv_nsec < 0) {
            ts.tv_nsec += 1000000000;
            ts.tv_sec  -= 1;
        }
        if (ts.tv_sec < 0 || ts.tv_nsec < 0) {
            errno = ETIMEDOUT;
            return -1;
        }
        /* Try to grab the semaphore. If the value was 0, this
         * will also change it to -1 */
        if (__android_sem_dec(&sem->count) > 0) {
            ANDROID_MEMBAR_FULL();
            break;
        }
        /* Contention detected. wait for a wakeup event */
        ret = android_futex_wait_ex(&sem->count, shared, shared|ANDROID_SEMCOUNT_MINUS_ONE, &ts);
        /* return in case of timeout or interrupt */
        if (ret == -ETIMEDOUT || ret == -EINTR) {
            errno = -ret;
            return -1;
        }
    }
    return 0;
}

/* Unlock a semaphore */
int android_sem_post(android_sem_t *sem) {
    unsigned int shared;
    int          old;
    if (sem == NULL)
        return EINVAL;
    shared = ANDROID_SEM_GET_SHARED(sem);
    ANDROID_MEMBAR_FULL();
    old = __android_sem_inc(&sem->count);
    if (old < 0) {
        /* contention on the semaphore, wake up all waiters */
        android_futex_wake_ex(&sem->count, shared, INT_MAX);
    }
    else if (old == ANDROID_SEM_MAX_VALUE) {
        /* overflow detected */
        errno = EOVERFLOW;
        return -1;
    }
    return 0;
}

int android_sem_trywait(android_sem_t *sem) {
    if (sem == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (__android_sem_trydec(&sem->count) > 0) {
        ANDROID_MEMBAR_FULL();
        return 0;
    } else {
        errno = EAGAIN;
        return -1;
    }
}

/* Note that Posix requires that sem_getvalue() returns, in
 * case of contention, the negative of the number of waiting
 * threads.
 *
 * However, code that depends on this negative value to be
 * meaningful is most probably racy. The GLibc sem_getvalue()
 * only returns the semaphore value, which is 0, in case of
 * contention, so we will mimick this behaviour here instead
 * for better compatibility.
 */
int android_sem_getvalue(android_sem_t *sem, int *sval) {
    int  val;
    if (sem == NULL || sval == NULL) {
        errno = EINVAL;
        return -1;
    }
    val = ANDROID_SEMCOUNT_TO_VALUE(sem->count);
    if (val < 0)
        val = 0;
    *sval = val;
    return 0;
}