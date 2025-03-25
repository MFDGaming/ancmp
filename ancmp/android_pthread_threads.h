#pragma once

#ifdef _WIN32

#include <windows.h>
#include "android_tls.h"

typedef struct {
    HANDLE thread;
    HANDLE tmp_wait_event;
    void *tls[ANDROID_BIONIC_TLS_SLOTS];
    BOOL is_detached;
    BOOL is_main_thread;
    void *(*start_func)(void *);
    void *start_arg;
} android_pthread_internal_t;

extern int android_thread_storage;

extern long volatile tls_free[];
extern void *volatile tls_destructors[];

BOOL android_threads_init();

#endif