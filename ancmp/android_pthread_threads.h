#pragma once

#ifdef _WIN32

#include <windows.h>
#include "android_tls.h"

typedef struct {
    HANDLE thread;
    DWORD thread_id;
    void *tls[ANDROID_BIONIC_TLS_SLOTS];
    BOOL is_detached;
} android_pthread_internal_t;

typedef struct android_threads {
    android_pthread_internal_t *thread;
    struct android_threads *next;
    struct android_threads *prev;
} android_threads_t;

extern unsigned char tls_free[];
extern void *tls_destructors[];
extern HANDLE tls_mutex;

BOOL android_threads_init();

BOOL android_threads_append(android_pthread_internal_t *thread);

android_pthread_internal_t *android_threads_get(DWORD thread_id);

BOOL android_threads_delete(android_pthread_internal_t *thread);

#endif