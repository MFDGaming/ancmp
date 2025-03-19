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

int android_pthread_equal(android_pthread_t one, android_pthread_t two) {
    return (one == two ? 1 : 0);
}

int android_pthread_detach(android_pthread_t thread) {
    pthread_t _thread = *(pthread_t *)&thread;
    return pthread_detach(_thread);
}

int android_pthread_setname_np(android_pthread_t thread, const char *name) {
    return 0;
}