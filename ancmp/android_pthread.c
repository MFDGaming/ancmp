#include "android_pthread.h"
#include "android_errno.h"
#include "android_futex.h"
#include "android_atomic.h"
#include <limits.h>
#include <string.h>
#include "android_tls.h"
#include "android_atomic.h"
#ifdef _WIN32
#include <windows.h>
#include "android_pthread_threads.h"
#else
#include <pthread.h>
#include <sched.h>
#endif

int android_pthread_equal(android_pthread_t one, android_pthread_t two) {
    return (one == two ? 1 : 0);
}

int android_pthread_setname_np(android_pthread_t thread, const char *name) {
    return 0;
}

int android_pthread_once(android_pthread_once_t *once_control, void (*init_routine)(void)) {
    android_pthread_once_t *ocptr = once_control;
    android_pthread_once_t value;
#define ONCE_INITIALIZING           (1 << 0)
#define ONCE_COMPLETED              (1 << 1)
    if ((*ocptr & ONCE_COMPLETED) != 0) {
        ANDROID_MEMBAR_FULL();
        return 0;
    }
    for (;;) {
        int32_t  oldval, newval;

        do {
            oldval = *ocptr;
            if ((oldval & ONCE_COMPLETED) != 0)
                break;

            newval = oldval | ONCE_INITIALIZING;
        } while (android_atomic_cmpxchg(oldval, newval, ocptr) != 0);

        if ((oldval & ONCE_COMPLETED) != 0) {
            ANDROID_MEMBAR_FULL();
            return 0;
        }

        if ((oldval & ONCE_INITIALIZING) == 0) {
            break;
        }
        android_futex_wait_ex(ocptr, 0, oldval, NULL);
    }
    (*init_routine)();
    ANDROID_MEMBAR_FULL();
    *ocptr = ONCE_COMPLETED;

    android_futex_wake_ex(ocptr, 0, INT_MAX);

    return 0;
}

#ifdef _WIN32

static int priority_conv(int policy, int priority) {
    if (policy == ANDROID_SCHED_FIFO) {
        if (priority <= 19)  return THREAD_PRIORITY_TIME_CRITICAL;
        if (priority <= 39)  return THREAD_PRIORITY_HIGHEST;
        if (priority <= 59)  return THREAD_PRIORITY_ABOVE_NORMAL;
        if (priority <= 79)  return THREAD_PRIORITY_NORMAL;
        return THREAD_PRIORITY_BELOW_NORMAL;
    }
    if (policy == ANDROID_SCHED_RR) {
        if (priority <= 19)  return THREAD_PRIORITY_HIGHEST;
        if (priority <= 39)  return THREAD_PRIORITY_ABOVE_NORMAL;
        if (priority <= 59)  return THREAD_PRIORITY_NORMAL;
        if (priority <= 79)  return THREAD_PRIORITY_BELOW_NORMAL;
        return THREAD_PRIORITY_LOWEST;
    }
    return THREAD_PRIORITY_NORMAL;
}

typedef struct {
    android_pthread_internal_t *thread;
    HANDLE *event;
    void *(*start_routine)(void *);
    void *arg;
} thread_wrapper_data_t;

static void android_pthread_call_destroy(android_pthread_internal_t *thread) {
    int i;
    for (i = 0; i < ANDROID_BIONIC_TLS_SLOTS; ++i) {
        if (InterlockedCompareExchange(&tls_free[i], 1, 1) == 1) {
            void *destructor = InterlockedCompareExchangePointer(&tls_destructors[i], NULL, NULL);
            if (destructor != NULL) {
                if (thread->tls[i] != NULL) {
                    ((void (*)(void *))destructor)(thread->tls[i]);
                }
            }
        }
    }
}

static DWORD WINAPI thread_wrapper(LPVOID lpParam) {
    android_pthread_internal_t *thread = (android_pthread_internal_t *)lpParam;
    DWORD ret;
    void *errno_alloc;

    if (!TlsSetValue(android_thread_storage, lpParam)) {
        return ANDROID_EACCES;
    }
    errno_alloc = calloc(1, sizeof(int));
    if (!errno_alloc) {
        return ANDROID_EACCES;
    }
    if (!TlsSetValue(android_errno_key, errno_alloc)) {
        free(errno_alloc);
        return ANDROID_EACCES;
    }
    thread->tls[ANDROID_TLS_SLOT_ERRNO] = errno_alloc;
    ret = (DWORD)thread->start_func(thread->start_arg);
    android_pthread_call_destroy(thread);
    if (thread->is_detached) {
        free(thread);
    }
    if (errno_alloc != NULL) {
        free(errno_alloc);
    }
    return ret;
}

#else

typedef struct {
    void *(*start_routine)(void *);
    void *arg;
} android_pthread_wrapper_args_t;

static void *thread_wrapper(void *arg) {
    android_pthread_wrapper_args_t args = *(android_pthread_wrapper_args_t *)arg;
    void *errno_alloc;
    void *ret;
    free(arg);
    errno_alloc = calloc(1, sizeof(int));
    if (!errno_alloc) {
        return (void *)ANDROID_EACCES;
    }
    if (pthread_setspecific(android_errno_key, errno_alloc) != 0) {
        free(errno_alloc);
        return (void *)ANDROID_EACCES;
    }
    ret = args.start_routine(args.arg);
    if (errno_alloc != NULL) {
        free(errno_alloc);
    }
    return ret;
}

#endif

int android_pthread_create(android_pthread_t *thread_out, android_pthread_attr_t const *attr, void *(*start_routine)(void *), void *arg) {
    android_pthread_attr_t attr_sv = android_gDefaultPthreadAttr;
#ifdef _WIN32
    android_pthread_internal_t *t;
#else
    pthread_attr_t real_attr;
    int real_policy = SCHED_OTHER;
    pthread_t thrd;
    android_pthread_wrapper_args_t *args;
#endif

    if (attr) {
        attr_sv = *attr;
    }
#ifdef _WIN32
    t = (android_pthread_internal_t *)malloc(sizeof(android_pthread_internal_t));
    if (t == NULL) {
        return ANDROID_EAGAIN;
    }
    t->is_detached = attr_sv.flags & ANDROID_PTHREAD_ATTR_FLAG_DETACHED;
    memset(t->tls, 0, sizeof(void *) * ANDROID_BIONIC_TLS_SLOTS);
    t->tls[ANDROID_TLS_SLOT_SELF] = (void *)t->tls;
    t->tls[ANDROID_TLS_SLOT_THREAD_ID] = (void *)t;
    t->is_main_thread = FALSE;
    t->start_func = start_routine;
    t->start_arg = arg;
    t->is_joined = 0;

    t->thread = CreateThread(NULL, attr_sv.stack_size, thread_wrapper, (void *)t, CREATE_SUSPENDED, NULL);
    if (t->thread == NULL) {
        free(t);
        return ANDROID_EAGAIN;
    }
    ANDROID_MEMBAR_FULL();
    SetThreadPriority(t->thread, priority_conv(attr_sv.sched_policy, attr_sv.sched_priority));
    ResumeThread(t->thread);
    *thread_out = (android_pthread_t)t;
    return 0;
#else
    args = (android_pthread_wrapper_args_t *)malloc(sizeof(android_pthread_wrapper_args_t));
    args->start_routine = start_routine;
    args->arg = arg;
    if (pthread_attr_init(&real_attr) != 0) {
        return ANDROID_EAGAIN;
    }
    if (attr_sv.sched_policy == ANDROID_SCHED_FIFO) {
        real_policy = SCHED_FIFO;
    } else if (attr_sv.sched_policy == ANDROID_SCHED_RR) {
        real_policy = SCHED_RR;
    }
    if (pthread_attr_setschedpolicy(&real_attr, real_policy) != 0) {
        pthread_attr_destroy(&real_attr);
        return ANDROID_EAGAIN;
    }
    if (real_policy == SCHED_FIFO || real_policy == SCHED_RR) {
        struct sched_param param;
        param.sched_priority = attr_sv.sched_priority;
        if (pthread_attr_setschedparam(&real_attr, &param) != 0) {
            pthread_attr_destroy(&real_attr);
            return ANDROID_EAGAIN;
        }
    }
    if (pthread_attr_setdetachstate(&real_attr, (attr_sv.flags & ANDROID_PTHREAD_ATTR_FLAG_DETACHED) ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE) != 0) {
        pthread_attr_destroy(&real_attr);
        return ANDROID_EAGAIN;
    }
    if (attr_sv.stack_base == NULL) {
        if (pthread_attr_setstacksize(&real_attr, attr_sv.stack_size) != 0) {
            pthread_attr_destroy(&real_attr);
            return ANDROID_EAGAIN;
        }
    } else {
        if (pthread_attr_setstack(&real_attr, attr_sv.stack_base, attr_sv.stack_size) != 0) {
            pthread_attr_destroy(&real_attr);
            return ANDROID_EAGAIN;
        }
    }
    if (pthread_attr_setguardsize(&real_attr, attr_sv.guard_size) != 0) {
        pthread_attr_destroy(&real_attr);
        return ANDROID_EAGAIN;
    }
    if (pthread_create(&thrd, &real_attr, thread_wrapper, (void *)args) != 0) {
        pthread_attr_destroy(&real_attr);
        return ANDROID_EAGAIN;
    }
    *(pthread_t *)thread_out = thrd;
    return 0;
#endif
}

int android_pthread_detach(android_pthread_t thid) {
#ifdef _WIN32
    android_pthread_internal_t *thrd = (android_pthread_internal_t *)thid;
    if (thrd == NULL) {
        return ANDROID_EINVAL;
    }
    if (!thrd->is_detached) {
        return ANDROID_EINVAL;
    }
    if (InterlockedCompareExchange(&thrd->is_joined, 1, 0)) {
        if (thrd->thread != NULL) {
            CloseHandle(thrd->thread);
        }
    }
    return 0;
#else
    pthread_t _thid = *(pthread_t *)&thid;
    return pthread_detach(_thid);
#endif
}

int android_pthread_join(android_pthread_t thid, void **ret_val) {
#ifdef _WIN32
    android_pthread_internal_t *thrd = (android_pthread_internal_t *)thid;
    if (thrd == NULL) {
        return ANDROID_EINVAL;
    }
    if (thrd->is_detached) {
        return ANDROID_EINVAL;
    }
    if (InterlockedCompareExchange(&thrd->is_joined, 1, 0) == 0) {
        if (thrd->thread != NULL) {
            int status;
            WaitForSingleObject(thrd->thread, INFINITE);
            GetExitCodeThread(thrd->thread, (LPDWORD)&status);
            CloseHandle(thrd->thread);
            if (ret_val) *ret_val = (void *)status;
            free(thrd);
            return 0;
        }
        free(thrd);
        return ANDROID_EINVAL;
    }
    return ANDROID_EINVAL;
#else
    pthread_t _thid = *(pthread_t *)&thid;
    return pthread_join(_thid, ret_val);
#endif
}

android_pthread_t android_pthread_self(void) {
#ifdef _WIN32
    return (android_pthread_t)TlsGetValue(android_thread_storage);
#else
    pthread_t thid = pthread_self();
    return *(android_pthread_t *)&thid;
#endif
}
