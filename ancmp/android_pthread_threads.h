#ifndef ANCMP_ANDROID_PTHREAD_THREADS_H
#define ANCMP_ANDROID_PTHREAD_THREADS_H

#ifdef _WIN32

#include <windows.h>
#include "android_tls.h"

typedef struct {
    HANDLE thread;
    void *tls[ANDROID_BIONIC_TLS_SLOTS];
    BOOL is_detached;
    BOOL is_main_thread;
    void *(*start_func)(void *);
    void *start_arg;
    LONG is_joined;
} android_pthread_internal_t;

extern int android_thread_storage;

extern long volatile tls_free[];
extern void *volatile tls_destructors[];

BOOL android_threads_init(void);

#endif

#endif
