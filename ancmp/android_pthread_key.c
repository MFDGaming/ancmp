#include "android_pthread_key.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
#include "android_errno.h"
#include "android_pthread_threads.h"
#include <stdio.h>
#include "android_atomic.h"

int android_pthread_key_create(android_pthread_key_t *key, void (*destructor_function)(void *)) {
#ifdef _WIN32
    int i;
#else
    pthread_key_t _key;
    int ret;
#endif

    if (key == NULL) {
        return ANDROID_EINVAL;
    }
#ifdef _WIN32
    for (i = 0; i < ANDROID_BIONIC_TLS_SLOTS; ++i) {
        if (InterlockedCompareExchange(&tls_free[i], 1, 0) == 0) {
            *key = i;
            InterlockedExchangePointer(&tls_destructors[i], (void *)destructor_function);
            return 0;
        }
    }
    return ANDROID_EAGAIN;
#else
    ret = pthread_key_create(&_key, destructor_function);
    *key = (android_pthread_key_t)(_key & 0xffffffff);
    return ret;
#endif
}

int android_pthread_key_delete(android_pthread_key_t key) {
#ifdef _WIN32
    if (key >= ANDROID_BIONIC_TLS_SLOTS || key < 0) {
        return ANDROID_EINVAL;
    }
    if (InterlockedCompareExchange(&tls_free[key], 0, 1) == 1) {
        InterlockedExchangePointer(&tls_destructors[key], NULL);
        return 0;
    }
    return ANDROID_EINVAL;
#else
    return pthread_key_delete((pthread_key_t)key);
#endif
}

void *android_pthread_getspecific(android_pthread_key_t key) {
#ifdef _WIN32
    void *ret = NULL;
    android_pthread_internal_t *thread;

    if (key >= ANDROID_BIONIC_TLS_SLOTS || key < 0) {
        puts("android_pthread_getspecific fail");
        return NULL;
    }
    thread = (android_pthread_internal_t *)TlsGetValue(android_thread_storage);
    if (thread == NULL) {
        puts("android_pthread_getspecific fail");
        return NULL;
    }
    if (InterlockedCompareExchange(&tls_free[key], 1, 1) == 1) {
        ANDROID_MEMBAR_FULL();
        ret = thread->tls[key];
    }
    /* puts("android_pthread_getspecific success");
    printf("%p\n", ret); */
    return ret;
#else
    return pthread_getspecific((pthread_key_t)key);
#endif
}

int android_pthread_setspecific(android_pthread_key_t key, const void *ptr) {
#ifdef _WIN32
    android_pthread_internal_t *thread;

    if (key >= ANDROID_BIONIC_TLS_SLOTS || key < 0) {
        return ANDROID_EINVAL;
    }
    thread = (android_pthread_internal_t *)TlsGetValue(android_thread_storage);
    if (thread == NULL) {
        return ANDROID_EAGAIN;
    }
    if (InterlockedCompareExchange(&tls_free[key], 1, 1) == 1) {
        thread->tls[key] = (void *)ptr;
        ANDROID_MEMBAR_FULL();
        return 0;
    }
    return ANDROID_EINVAL;
#else
    return pthread_setspecific((pthread_key_t)key, ptr);
#endif
}
