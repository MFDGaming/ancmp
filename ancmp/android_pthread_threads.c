#ifdef _WIN32

#include "android_pthread_threads.h"
#include <windows.h>
#include "android_atomic.h"

long volatile tls_free[ANDROID_BIONIC_TLS_SLOTS];
void *volatile tls_destructors[ANDROID_BIONIC_TLS_SLOTS];

int android_thread_storage = TLS_OUT_OF_INDEXES;

BOOL android_threads_init(void) {
    android_pthread_internal_t *thread = (android_pthread_internal_t *)malloc(sizeof(android_pthread_internal_t));
    if (thread == NULL) {
        return FALSE;
    }
    android_thread_storage = TlsAlloc();
    if (android_thread_storage == TLS_OUT_OF_INDEXES) {
        free(thread);
        return FALSE;
    }
    thread->is_detached = 0;
    thread->is_main_thread = TRUE;
    memset(thread->tls, 0, sizeof(void *) * ANDROID_BIONIC_TLS_SLOTS);
    ANDROID_MEMBAR_FULL();

    TlsSetValue(android_thread_storage, (void *)thread);
    memset((void *)tls_destructors, 0, sizeof(tls_destructors));
    memset((void *)tls_free, 0, sizeof(tls_free));
    tls_free[ANDROID_TLS_SLOT_SELF] = 1;
    tls_free[ANDROID_TLS_SLOT_THREAD_ID] = 1;
    tls_free[ANDROID_TLS_SLOT_ERRNO] = 1;
    tls_free[ANDROID_TLS_SLOT_OPENGL] = 1;
    tls_free[ANDROID_TLS_SLOT_OPENGL_API] = 1;
    return TRUE;
}

#endif
