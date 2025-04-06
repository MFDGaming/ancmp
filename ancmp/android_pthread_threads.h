#ifndef ANCMP_ANDROID_PTHREAD_THREADS_H
#define ANCMP_ANDROID_PTHREAD_THREADS_H

#ifdef _WIN32

#include <windows.h>
#include "android_tls.h"

typedef struct _android_pthread_destructor_t {
    struct _android_pthread_destructor_t *next;
    struct _android_pthread_destructor_t *prev;
    void (*destructor_function)(void *);
    DWORD tls_index;
} android_pthread_destructor_t;

typedef struct {
    HANDLE thread;
    BOOL is_detached;
    BOOL is_main_thread;
    void *(*start_func)(void *);
    void *start_arg;
    LONG is_joined;
} android_pthread_internal_t;

extern android_pthread_destructor_t *android_pthread_destructors;
extern HANDLE android_pthread_destructors_mutex;

extern int android_thread_storage;

extern long volatile tls_free[];
extern void *volatile tls_destructors[];

BOOL android_pthread_destructors_push(DWORD tls_index, void (*destructor_function)(void *));
void android_pthread_destructors_remove(DWORD tls_index);

BOOL android_threads_init(void);

#endif

#endif
