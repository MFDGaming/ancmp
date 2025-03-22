#include "android_pthread_key.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
#include "android_errno.h"
#include "android_pthread_threads.h"

int android_pthread_key_create(android_pthread_key_t *key, void (*destructor_function)(void *)) {
    if (key == NULL) {
        return ANDROID_EINVAL;
    }
#ifdef _WIN32
    WaitForSingleObject(tls_mutex, INFINITE);
    for (int i = 0; i < ANDROID_BIONIC_TLS_SLOTS; ++i) {
        if (tls_free[i] == 0) {
            tls_free[i] = 1;
            *key = i;
            tls_destructors[i] = (void *)destructor_function;
            ReleaseMutex(tls_mutex);
            return 0;
        }
    }
    ReleaseMutex(tls_mutex);
    return ANDROID_EAGAIN;
#else
    pthread_key_t _key;
    int ret = pthread_key_create(&_key, destructor_function);
    *key = (android_pthread_key_t)(_key & 0xffffffff);
    return ret;
#endif
}

int android_pthread_key_delete(android_pthread_key_t key) {
#ifdef _WIN32
    if (key >= ANDROID_BIONIC_TLS_SLOTS || key < 0) {
        return ANDROID_EINVAL;
    }
    WaitForSingleObject(tls_mutex, INFINITE);
    tls_free[key] = 0;
    tls_destructors[key] = NULL;
    ReleaseMutex(tls_mutex);
    return 0;
#else
    return pthread_key_delete((pthread_key_t)key);
#endif
}

void *android_pthread_getspecific(android_pthread_key_t key) {
#ifdef _WIN32
    if (key >= ANDROID_BIONIC_TLS_SLOTS || key < 0) {
        return NULL;
    }
    android_pthread_internal_t *thread = android_threads_get(GetCurrentThreadId());
    if (thread == NULL) {
        return NULL;
    }
    void *ret = NULL;
    WaitForSingleObject(tls_mutex, INFINITE);
    if (tls_free[key] == 1) {
        ret = thread->tls[key];
    }
    ReleaseMutex(tls_mutex);
    return ret;
#else
    return pthread_getspecific((pthread_key_t)key);
#endif
}

int android_pthread_setspecific(android_pthread_key_t key, const void *ptr) {
#ifdef _WIN32
    if (key >= ANDROID_BIONIC_TLS_SLOTS || key < 0) {
        return ANDROID_EINVAL;
    }
    android_pthread_internal_t *thread = android_threads_get(GetCurrentThreadId());
    if (thread == NULL) {
        return ANDROID_EAGAIN;
    }
    int ret = ANDROID_EINVAL;
    WaitForSingleObject(tls_mutex, INFINITE);
    if (tls_free[key] == 1) {
        thread->tls[key] = (void *)ptr;
        ret = 0;
    }
    ReleaseMutex(tls_mutex);
    return ret;
#else
    return pthread_setspecific((pthread_key_t)key, ptr);
#endif
}