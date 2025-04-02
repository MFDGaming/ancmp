#ifndef ANCMP_ANDROID_PTHREAD_H
#define ANCMP_ANDROID_PTHREAD_H

#include "android_pthread_attr.h"
#include "android_pthread_mutex.h"
#include "android_pthread_cond.h"
#include "android_pthread_key.h"

typedef long android_pthread_t;
typedef volatile int android_pthread_once_t;

int android_pthread_equal(android_pthread_t one, android_pthread_t two);

int android_pthread_setname_np(android_pthread_t thread, const char *name);

int android_pthread_once(android_pthread_once_t *once_control, void (*init_routine)(void));

int android_pthread_create(android_pthread_t *thread_out, android_pthread_attr_t const *attr, void *(*start_routine)(void *), void *arg);

int android_pthread_detach(android_pthread_t thid);

int android_pthread_join(android_pthread_t thid, void **ret_val);

android_pthread_t android_pthread_self(void);

#endif
