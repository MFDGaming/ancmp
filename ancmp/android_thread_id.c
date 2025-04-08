#include "android_thread_id.h"
#include "android_atomic.h"
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <time.h>
#endif


#if defined(_WIN32) && defined(_MSC_VER)
#define ANDROID_THREAD_ID_LIST_TYPE unsigned long
#else
#define ANDROID_THREAD_ID_LIST_TYPE unsigned int
#endif

#define ANDROID_THREAD_ID_SHIFT (sizeof(ANDROID_THREAD_ID_LIST_TYPE) + 1)
#define ANDROID_THREAD_ID_MASK ((1 << ANDROID_THREAD_ID_SHIFT) - 1)
#define ANDROID_THREAD_ID_LIST_SIZE (0x10000 >> ANDROID_THREAD_ID_SHIFT)

volatile ANDROID_THREAD_ID_LIST_TYPE android_thread_id_free_list[ANDROID_THREAD_ID_LIST_SIZE];

#ifdef _WIN32
DWORD android_thread_id_storage;
#else
pthread_key_t android_thread_id_storage;
#endif

int android_thread_id_init(void) {
    memset((void *)android_thread_id_free_list, 0xff, sizeof(android_thread_id_free_list));
    android_thread_id_alloc(1);
#ifdef _WIN32
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
    if (pthread_setspecific(android_thread_id_storage, (void *)1) != 0) {
        pthread_key_delete(android_thread_id_storage);
        return 0;
    }
#endif
    return 1;
}

int android_thread_id_alloc(int thread_id) {
    int old_value, new_value;
    do {
        old_value = android_thread_id_free_list[thread_id >> ANDROID_THREAD_ID_SHIFT];
        
        if ((old_value & (1 << (thread_id & ANDROID_THREAD_ID_MASK))) == 0) {
            return 0;
        }
        new_value = old_value & ~(1 << (thread_id & ANDROID_THREAD_ID_MASK));

    } while (android_atomic_cmpxchg(old_value, new_value, (volatile int *)&android_thread_id_free_list[thread_id >> ANDROID_THREAD_ID_SHIFT]));
    return 1;
}

void android_thread_id_free(int thread_id) {
    android_atomic_or(1 << (thread_id & ANDROID_THREAD_ID_MASK), (volatile int *)&android_thread_id_free_list[thread_id >> ANDROID_THREAD_ID_SHIFT]);
}

int android_thread_id_find_free(void) {
    int thread_id;
    for (thread_id = 2; thread_id <= 0xffff; ++thread_id) {
        if (android_thread_id_alloc(thread_id)) {
            return thread_id;
        }
    }
    return -1;
}

int android_thread_id_acquire(void) {
    int i;
#ifndef _WIN32
    struct timespec ts;
    ts.tv_sec = 5;
    ts.tv_nsec = 0;
#endif
    for (i = 0; i < 10; ++i) {
        int thread_id = android_thread_id_find_free();
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
