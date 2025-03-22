#ifdef _WIN32

#include "android_pthread_threads.h"
#include <windows.h>

unsigned char tls_free[ANDROID_BIONIC_TLS_SLOTS];
void *tls_destructors[ANDROID_BIONIC_TLS_SLOTS];
HANDLE tls_mutex = NULL;

android_threads_t *android_threads_list = NULL;
HANDLE android_threads_mutex = NULL;

BOOL android_threads_init() {
    android_threads_mutex = CreateMutex(NULL, FALSE, NULL);
    if (android_threads_mutex == NULL) {
        return FALSE;
    }
    tls_mutex = CreateMutex(NULL, FALSE, NULL);
    if (tls_mutex == NULL) {
        return FALSE;
    }
    android_pthread_internal_t *thread = (android_pthread_internal_t *)malloc(sizeof(android_pthread_internal_t));
    thread->is_detached = 0;
    thread->thread = GetCurrentThread();
    thread->thread_id = GetCurrentThreadId();
    android_threads_append(thread);
    memset(tls_destructors, 0, sizeof(tls_destructors));
    memset(tls_free, 0, sizeof(tls_free));
    tls_free[ANDROID_TLS_SLOT_SELF] = 1;
    tls_free[ANDROID_TLS_SLOT_THREAD_ID] = 1;
    tls_free[ANDROID_TLS_SLOT_ERRNO] = 1;
    tls_free[ANDROID_TLS_SLOT_OPENGL] = 1;
    tls_free[ANDROID_TLS_SLOT_OPENGL_API] = 1;
    return TRUE;
}

BOOL android_threads_append(android_pthread_internal_t *thread) {
    android_threads_t *entry = (android_threads_t *)malloc(sizeof(android_threads_t));
    if (!entry) {
        return FALSE;
    }
    entry->thread = thread;
    WaitForSingleObject(android_threads_mutex, INFINITE);
    entry->prev = NULL;
    entry->next = android_threads_list;
    if (android_threads_list) {
        android_threads_list->prev = entry;
    }
    android_threads_list = entry;
    ReleaseMutex(android_threads_mutex);
    return TRUE;
}

android_pthread_internal_t *android_threads_get(DWORD thread_id) {
    WaitForSingleObject(android_threads_mutex, INFINITE);
    android_pthread_internal_t *ret = NULL;
    for (android_threads_t *entry = android_threads_list; entry != NULL; entry = entry->next) {
        if (entry->thread->thread_id == thread_id) {
            ret = entry->thread;
            break;
        }
    }
    ReleaseMutex(android_threads_mutex);
    return ret;
}

BOOL android_threads_delete(android_pthread_internal_t *thread) {
    WaitForSingleObject(android_threads_mutex, INFINITE);
    for (android_threads_t *entry = android_threads_list; entry != NULL; entry = entry->next) {
        if (entry->thread == thread) {
            if (entry->prev == NULL && entry->next == NULL) {
                android_threads_list = NULL;
            } else if (entry->prev == NULL) {
                android_threads_list = entry->next;
                entry->next->prev = NULL;
            } else if (entry->next == NULL) {
                entry->prev->next = NULL;
            } else {
                entry->prev->next = entry->next;
                entry->next->prev = entry->prev;
            }
            free(entry);
            ReleaseMutex(android_threads_mutex);
            return TRUE;
        }
    }
    ReleaseMutex(android_threads_mutex);
    return FALSE;
}

#endif