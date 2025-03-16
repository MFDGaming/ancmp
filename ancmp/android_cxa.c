#include "android_cxa.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

typedef struct {
  long int flavor;
  union {
    void (*at)(void);
    struct {
      void (*fn)(int status, void *arg);
      void *arg;
    } on;
    struct {
      void (*fn)(void *arg, int status);
      void *arg;
      void *dso_handle;
    } cxa;
  } func;
} android_exit_function_t;

typedef struct android_exit_function_list {
  struct android_exit_function_list *next;
  size_t idx;
  android_exit_function_t fns[32];
} android_exit_function_list_t;

enum {
    android_ef_free,
    android_ef_us,
    android_ef_on,
    android_ef_at,
    android_ef_cxa
};

#ifdef _WIN32
CRITICAL_SECTION lock;
LONG lock_inited = 0;
#else
static pthread_mutex_t lock =

#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER

PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
#else

#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#else
PTHREAD_MUTEX_INITIALIZER;
#endif

#endif

#endif

static android_exit_function_list_t initial;
android_exit_function_list_t *__exit_funcs = &initial;

android_exit_function_t *android_new_exitfn() {
    android_exit_function_list_t *l;
    size_t i = 0;
#ifdef _WIN32
    if (!InterlockedCompareExchange(&lock_inited, 1, 0)) {
        InitializeCriticalSection(&lock);
    }
    EnterCriticalSection(&lock);
#else
    pthread_mutex_lock(&lock);
#endif
    for (l = __exit_funcs; l != NULL; l = l->next) {
        for (i = 0; i < l->idx; ++i) {
            if (l->fns[i].flavor == android_ef_free) {
                break;
            }
        }
        if ( i < l->idx ) {
            break;
        }
        if (l->idx < sizeof (l->fns) / sizeof (l->fns[0])) {
            i = l->idx++;
            break;
        }
    }
    if (l == NULL) {
        l = (android_exit_function_list_t *)malloc(sizeof(android_exit_function_list_t));
        if (l != NULL) {
            l->next = __exit_funcs;
            __exit_funcs = l;
            l->idx = 1;
            i = 0;
        }
    }
    if (l != NULL) {
        l->fns[i].flavor = android_ef_us;
    }
#ifdef _WIN32
    LeaveCriticalSection(&lock);
#else
    pthread_mutex_unlock(&lock);
#endif
    return l == NULL ? NULL : &l->fns[i];
}

int android_cxa_atexit(void (*func)(void *), void *arg, void *d) {
    android_exit_function_t *new = android_new_exitfn();
    if (new == NULL) {
        return -1;
    }
    new->flavor = android_ef_cxa;
    new->func.cxa.fn = (void (*) (void *, int)) func;
    new->func.cxa.arg = arg;
    new->func.cxa.dso_handle = d;
    return 0;
}

void android_cxa_finalize(void *d) {
    android_exit_function_list_t *funcs;
#ifdef _WIN32
    if (!InterlockedCompareExchange(&lock_inited, 1, 0)) {
        InitializeCriticalSection(&lock);
    }
    EnterCriticalSection(&lock);
#else
    pthread_mutex_lock(&lock);
#endif
    for (funcs = __exit_funcs; funcs; funcs = funcs->next) {
        android_exit_function_t *f;
        for (f = &funcs->fns[funcs->idx - 1]; f >= &funcs->fns[0]; --f) {
            if ((d == NULL || d == f->func.cxa.dso_handle) && (f->flavor == android_ef_cxa)) {
                f->flavor = android_ef_free;
                (*f->func.cxa.fn)(f->func.cxa.arg, 0);
            }
        }
    }
#ifdef _WIN32
    LeaveCriticalSection(&lock);
#else
    pthread_mutex_unlock(&lock);
#endif
}

void android_cxa_pure_virtual() {
    puts("Pure virtual function called. Are you calling virtual methods from a destructor?");
    abort();
}