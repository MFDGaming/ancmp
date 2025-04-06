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
    DWORD tls_index;
#else
    pthread_key_t _key;
    int ret;
#endif

    if (key == NULL) {
        return ANDROID_EINVAL;
    }
#ifdef _WIN32
    tls_index = TlsAlloc();
    if (tls_index == TLS_OUT_OF_INDEXES) {
        return ANDROID_EAGAIN;
    }
    *key = (android_pthread_key_t)tls_index;
    android_pthread_destructors_push(tls_index, destructor_function);
    return 0;
#else
    ret = pthread_key_create(&_key, destructor_function);
    *key = (android_pthread_key_t)(_key & 0xffffffff);
    return ret;
#endif
}

int android_pthread_key_delete(android_pthread_key_t key) {
#ifdef _WIN32
    if (key == TLS_OUT_OF_INDEXES) {
        return ANDROID_EINVAL;
    }
    if (!TlsFree((DWORD)key)) {
        return ANDROID_EINVAL;
    }
    android_pthread_destructors_remove((DWORD)key);
    return 0;
#else
    return pthread_key_delete((pthread_key_t)key);
#endif
}

void *android_pthread_getspecific(android_pthread_key_t key) {
#ifdef _WIN32
    if (key == TLS_OUT_OF_INDEXES) {
        return NULL;
    }
    return TlsGetValue((DWORD)key);
#else
    return pthread_getspecific((pthread_key_t)key);
#endif
}

int android_pthread_setspecific(android_pthread_key_t key, const void *ptr) {
#ifdef _WIN32
    if (key == TLS_OUT_OF_INDEXES) {
        return ANDROID_EINVAL;
    }
    if (!TlsSetValue((DWORD)key, (void *)ptr)) {
        return ANDROID_EINVAL;
    }
    return 0;
#else
    return pthread_setspecific((pthread_key_t)key, ptr);
#endif
}
