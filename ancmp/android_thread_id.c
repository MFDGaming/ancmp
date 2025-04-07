#include "android_thread_id.h"
#include "android_atomic.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <time.h>
#endif

volatile int android_thread_id = 2;

typedef struct _android_reusable_thread_id_t {
    struct _android_reusable_thread_id_t *next;
    int thread_id;
} android_reusable_thread_id_t;

android_reusable_thread_id_t *android_reusable_thread_ids = NULL;

#ifdef _WIN32
HANDLE android_reusable_thread_id_mutex;
DWORD android_thread_id_storage;
#else
pthread_mutex_t android_reusable_thread_id_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_key_t android_thread_id_storage;
#endif


int android_thread_id_init(void) {
#ifdef _WIN32
    android_reusable_thread_id_mutex = CreateMutex(NULL, FALSE, NULL);
    if (android_reusable_thread_id_mutex == NULL) {
        return 0;
    }
    android_thread_id_storage = TlsAlloc();
    if (android_thread_id_storage == TLS_OUT_OF_INDEXES) {
        return 0;
    }
    if (!TlsSetValue(android_thread_id_storage, (void *)1)) {
        TlsFree(android_thread_id_storage);
        return 0;
    }
#else
    if (pthread_key_create(&android_thread_id_storage, NULL) != 0) {
        return 0;
    }
    if (pthread_setspecific(android_thread_id_storage, 1) != 0) {
        pthread_key_delete(android_thread_id_storage);
        return 0;
    }
#endif
    return 1;
}

int android_reusable_thread_id_pop(void) {
    int thread_id = -1;
#ifdef _WIN32
    WaitForSingleObject(android_reusable_thread_id_mutex, INFINITE);
#else
    pthread_mutex_lock(&android_reusable_thread_id_mutex);
#endif
    if (android_reusable_thread_ids != NULL) {
        android_reusable_thread_id_t *current = android_reusable_thread_ids;
        android_reusable_thread_ids = android_reusable_thread_ids->next;
        thread_id = current->thread_id;
        free(current);
    }
#ifdef _WIN32
    ReleaseMutex(android_reusable_thread_id_mutex);
#else
    pthread_mutex_unlock(&android_reusable_thread_id_mutex);
#endif
    return thread_id;
}

int android_reusable_thread_id_push(int thread_id) {
    android_reusable_thread_id_t *entry = (android_reusable_thread_id_t *)malloc(sizeof(android_reusable_thread_id_t));
    if (!entry) {
        return 0;
    }
    entry->thread_id = thread_id;
#ifdef _WIN32
    WaitForSingleObject(android_reusable_thread_id_mutex, INFINITE);
#else
    pthread_mutex_lock(&android_reusable_thread_id_mutex);
#endif
    entry->next = android_reusable_thread_ids;
    android_reusable_thread_ids = entry;
#ifdef _WIN32
    ReleaseMutex(android_reusable_thread_id_mutex);
#else
    pthread_mutex_unlock(&android_reusable_thread_id_mutex);
#endif
    return 1;
}
#include <stdio.h>
int android_thread_id_new(void) {
    int thread_id = android_atomic_inc(&android_thread_id);
    if (thread_id <= 0xffff) {
        return thread_id;
    }
    android_atomic_swap(0x10000, &android_thread_id);
    return android_reusable_thread_id_pop();
}

int android_thread_id_acquire(void) {
    int i;
#ifndef _WIN32
    struct timespec ts;
    ts.tv_sec = 5;
    ts.tv_nsec = 0;
#endif
    for (i = 0; i < 10; ++i) {
        int thread_id = android_thread_id_new();
        if (thread_id != -1) {
#ifdef _WIN32
            TlsSetValue(android_thread_id_storage, (void *)thread_id);
#else
            pthread_setspecific(android_thread_id_storage, (void *)thread_id);
#endif
            return thread_id;
        }
#ifdef _WIN32
        Sleep(5000);
#else
        nanosleep(&ts, NULL);
#endif
    }
    return -1;
}

int android_thread_id_get_current(void) {
#ifdef _WIN32
    return (int)TlsGetValue(android_thread_id_storage);
#else
    return (int)pthread_getspecific(android_thread_id_storage);
#endif
}