#pragma once

#include "android_pthread_mutex.h"
#include <time.h>

typedef struct {
    int volatile value;
} android_pthread_cond_t;

typedef long android_pthread_condattr_t;

#define ANDROID_COND_SHARED_MASK        0x0001
#define ANDROID_COND_COUNTER_INCREMENT  0x0002
#define ANDROID_COND_COUNTER_MASK       (~ANDROID_COND_SHARED_MASK)

#define ANDROID_COND_IS_SHARED(c)  (((c)->value & ANDROID_COND_SHARED_MASK) != 0)

#define ANDROID_PTHREAD_COND_INITIALIZER  {0}

int android_pthread_cond_init(android_pthread_cond_t *cond, const android_pthread_condattr_t *attr);

int android_pthread_cond_destroy(android_pthread_cond_t *cond);

int android_pthread_cond_broadcast(android_pthread_cond_t *cond);

int android_pthread_cond_signal(android_pthread_cond_t *cond);

int android_pthread_cond_timedwait(android_pthread_cond_t *cond, android_pthread_mutex_t *mutex, const struct timespec *abstime);

int android_pthread_cond_wait(android_pthread_cond_t *cond, android_pthread_mutex_t *mutex);

int android_pthread_condattr_init(android_pthread_condattr_t *attr);

int android_pthread_condattr_destroy(android_pthread_condattr_t *attr);

int android_pthread_condattr_getpshared(const android_pthread_condattr_t *attr, int *pshared);

int android_pthread_condattr_setpshared(android_pthread_condattr_t *attr, int pshared);