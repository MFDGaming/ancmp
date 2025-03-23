#include "android_pthread.h"
#include "android_errno.h"
#include "android_futex.h"
#include "android_atomic.h"
#include <limits.h>
#include <string.h>
#include "android_tls.h"
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
    volatile android_pthread_once_t *ocptr = once_control;
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

DWORD WINAPI thread_wrapper(LPVOID lpParam) {
    thread_wrapper_data_t *data = (thread_wrapper_data_t *)lpParam;
    data->thread->thread_id = GetCurrentThreadId();
    SetEvent(data->event);
    return (DWORD)data->start_routine(data->arg);
}

#endif

int android_pthread_create(android_pthread_t *thread_out, android_pthread_attr_t const *attr, void *(*start_routine)(void *), void *arg) {
    android_pthread_attr_t attr_sv = android_gDefaultPthreadAttr;
    if (attr) {
        attr_sv = *attr;
    }
#ifdef _WIN32
    android_pthread_internal_t *t = (android_pthread_internal_t *)malloc(sizeof(android_pthread_internal_t));
    if (t == NULL) {
        return ANDROID_EAGAIN;
    }
    t->is_detached = attr_sv.flags & ANDROID_PTHREAD_ATTR_FLAG_DETACHED;
    memset(t->tls, 0, sizeof(void *) * ANDROID_BIONIC_TLS_SLOTS);
    t->tls[ANDROID_TLS_SLOT_SELF] = (void *)t->tls;
    t->tls[ANDROID_TLS_SLOT_THREAD_ID] = (void *)t;

    thread_wrapper_data_t *wrapper_data = (thread_wrapper_data_t *)malloc(sizeof(thread_wrapper_data_t));
    wrapper_data->thread = t;
    wrapper_data->start_routine = start_routine;
    wrapper_data->arg = arg;
    wrapper_data->event = CreateEvent(NULL, FALSE, FALSE, NULL);

    t->thread = CreateThread(NULL, attr_sv.stack_size, thread_wrapper, wrapper_data, CREATE_SUSPENDED | STACK_SIZE_PARAM_IS_A_RESERVATION, NULL);
    if (t->thread == NULL) {
        free(t);
        return ANDROID_EAGAIN;
    }
    SetThreadPriority(t->thread, priority_conv(attr_sv.sched_policy, attr_sv.sched_priority));
    android_threads_append(t);
    ResumeThread(t->thread);
    WaitForSingleObject(wrapper_data->event, INFINITE);
    CloseHandle(wrapper_data->event);
    free(wrapper_data);
    *thread_out = (android_pthread_t)t;
    return 0;
#else
    pthread_attr_t real_attr;
    if (pthread_attr_init(&real_attr) != 0) {
        return ANDROID_EAGAIN;
    }
    int real_policy = SCHED_OTHER;
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
    pthread_t thrd;
    if (pthread_create(&thrd, &real_attr, start_routine, arg) != 0) {
        pthread_attr_destroy(&real_attr);
        return ANDROID_EAGAIN;
    }
    *(pthread_t *)thread_out = thrd;
    return 0;
#endif
}

#ifdef _WIN32
static void android_pthread_call_destroy(android_pthread_internal_t *thread) {
    for (int i = 0; i < ANDROID_BIONIC_TLS_SLOTS; ++i) {
        void *destructor = NULL;
        void *val = NULL;
        WaitForSingleObject(tls_mutex, INFINITE);
        if (tls_free[i] == 1) {
            if (tls_destructors[i] != NULL) {
                if (thread->tls[i] != NULL) {
                    val = thread->tls[i];
                    destructor = tls_destructors[i];
                }
            }
        }
        ReleaseMutex(tls_mutex);
        if (destructor) {
            ((void (*)(void *))destructor)(val);
        }
    }
}
#endif

int android_pthread_detach(android_pthread_t thid) {
#ifdef _WIN32
    android_pthread_internal_t *thrd = (android_pthread_internal_t *)thid;
    if (thrd == NULL) {
        return ANDROID_EINVAL;
    }
    if (thrd->thread != NULL) {
        CloseHandle(thrd->thread);
        android_pthread_call_destroy(thrd);
    }
    free(thrd);
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
    if (thrd->thread != NULL) {
        WaitForSingleObject(thrd->thread, INFINITE);
        int status;
        GetExitCodeThread(thrd->thread, (LPDWORD)&status);
        CloseHandle(thrd->thread);
        if (ret_val) *ret_val = (void *)status;
        android_pthread_call_destroy(thrd);
    }
    free(thrd);
    return 0;
#else
    pthread_t _thid = *(pthread_t *)&thid;
    return pthread_join(_thid, ret_val);
#endif
}

android_pthread_t android_pthread_self(void) {
#ifdef _WIN32
    return (android_pthread_t)android_threads_get(GetCurrentThreadId());
#else
    pthread_t thid = pthread_self();
    return *(android_pthread_t *)&thid;
#endif
}