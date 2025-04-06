#ifdef _WIN32

#include "android_pthread_threads.h"
#include <windows.h>
#include "android_atomic.h"

long volatile tls_free[ANDROID_BIONIC_TLS_SLOTS];
void *volatile tls_destructors[ANDROID_BIONIC_TLS_SLOTS];

int android_thread_storage = TLS_OUT_OF_INDEXES;
android_pthread_destructor_t *android_pthread_destructors = NULL;
HANDLE android_pthread_destructors_mutex = NULL;

BOOL android_pthread_destructors_push(DWORD tls_index, void (*destructor_function)(void *)) {
    android_pthread_destructor_t *current;

    if (tls_index == TLS_OUT_OF_INDEXES || destructor_function == NULL) {
        return FALSE;
    }
    current = (android_pthread_destructor_t *)malloc(sizeof(android_pthread_destructor_t));
    current->destructor_function = destructor_function;
    current->tls_index = tls_index;
    current->prev = NULL;
    WaitForSingleObject(android_pthread_destructors_mutex, INFINITE);
    if (android_pthread_destructors != NULL) {
        android_pthread_destructors->prev = current;
    }
    current->next = android_pthread_destructors;
    android_pthread_destructors = current;
    ReleaseMutex(android_pthread_destructors_mutex);
    return TRUE;
}

void android_pthread_destructors_remove(DWORD tls_index) {
    android_pthread_destructor_t *current;

    if (tls_index == TLS_OUT_OF_INDEXES) {
        return;
    }
    WaitForSingleObject(android_pthread_destructors_mutex, INFINITE);
    for (current = android_pthread_destructors; current != NULL; current = current->next) {
        if (current->tls_index == tls_index) {
            if (current->prev == NULL && current->next == NULL) {
                android_pthread_destructors = NULL;
            } else if (current->prev == NULL) {
                android_pthread_destructors = current->next;
                android_pthread_destructors->prev = NULL;
            } else if (current->next == NULL) {
                current->prev->next = NULL;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            free(current);
            break;
        }
    }
    ReleaseMutex(android_pthread_destructors_mutex);
}

BOOL android_threads_init(void) {
    android_pthread_internal_t *thread = (android_pthread_internal_t *)malloc(sizeof(android_pthread_internal_t));
    if (thread == NULL) {
        return FALSE;
    }
    android_pthread_destructors_mutex = CreateMutex(NULL, FALSE, NULL);
    if (android_pthread_destructors_mutex == NULL) {
        free(thread);
        return FALSE;
    }
    android_thread_storage = TlsAlloc();
    if (android_thread_storage == TLS_OUT_OF_INDEXES) {
        CloseHandle(android_pthread_destructors_mutex);
        free(thread);
        return FALSE;
    }
    thread->is_detached = 0;
    thread->is_main_thread = TRUE;

    ANDROID_MEMBAR_FULL();

    TlsSetValue(android_thread_storage, (void *)thread);
    return TRUE;
}

#endif
