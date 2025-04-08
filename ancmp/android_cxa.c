#include "android_cxa.h"
#include <stdlib.h>
#include <stdio.h>
#include "android_pthread.h"
#include "android_atomic.h"

typedef struct _android_cxa_atexit_t {
    struct _android_cxa_atexit_t *next;
    struct _android_cxa_atexit_t *prev;
    void (*func)(void *);
    void *arg;
    void *dso;
    int called;
} android_cxa_atexit_t;

static long android_dso_handle_d = 0xd50;

void *android_dso_handle = (void *)&android_dso_handle_d;

static android_pthread_mutex_t lock = ANDROID_PTHREAD_MUTEX_INITIALIZER;

static android_cxa_atexit_t *android_cxa_atexit_arr = NULL;

int android_cxa_atexit(void (*func)(void *), void *arg, void *dso) {
    android_cxa_atexit_t *f = (android_cxa_atexit_t *)malloc(sizeof(android_cxa_atexit_t));
    if (!f) {
        return -1;
    }
    f->func = func;
    f->arg = arg;
    f->dso = dso;
    f->prev = NULL;
    f->called = 0;
    android_pthread_mutex_lock(&lock);
    if (android_cxa_atexit_arr) {
        android_cxa_atexit_arr->prev = f;
    }
    f->next = android_cxa_atexit_arr;
    android_cxa_atexit_arr = f;
    android_pthread_mutex_unlock(&lock);
    return 0;
}

void android_cxa_finalize(void *dso) {
    android_cxa_atexit_t *current;
    for (;;) {
        android_cxa_atexit_t *entry;

        android_pthread_mutex_lock(&lock);

        for (entry = android_cxa_atexit_arr; entry != NULL; entry = entry->next) {
            if ((dso == NULL || dso == entry->dso) && !android_atomic_cmpxchg(0, 1, &entry->called)) {
                void (*func)(void *) = entry->func;
                void *arg = entry->arg;
                
                if (entry->prev == NULL && entry->next == NULL) {
                    android_cxa_atexit_arr = NULL;
                } else if (entry->prev == NULL) {
                    android_cxa_atexit_arr = entry->next;
                    android_cxa_atexit_arr->prev = NULL;
                } else if (entry->next == NULL) {
                    entry->prev->next = NULL;
                } else {
                    entry->prev->next = entry->next;
                    entry->next->prev = entry->prev;
                }
                free(entry);
                android_pthread_mutex_unlock(&lock);
                func(arg);
                goto restart;
            }
        }
        android_pthread_mutex_unlock(&lock);
        return;
    restart:
        continue;
    }
}

void android_cxa_pure_virtual(void) {
    puts("Pure virtual function called. Are you calling virtual methods from a destructor?");
    abort();
}
