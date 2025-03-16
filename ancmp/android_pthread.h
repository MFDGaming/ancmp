#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "android_sched.h"
#include "android_pthread_mutex.h"
#include "android_pthread_cond.h"

#define ANDROID_PTHREAD_CREATE_DETACHED  0x00000001
#define ANDROID_PTHREAD_CREATE_JOINABLE  0x00000000

#define ANDROID_PTHREAD_ONCE_INIT    0

#define ANDROID_PTHREAD_PROCESS_PRIVATE  0
#define ANDROID_PTHREAD_PROCESS_SHARED   1

#define ANDROID_PTHREAD_SCOPE_SYSTEM     0
#define ANDROID_PTHREAD_SCOPE_PROCESS    1

#define ANDROID_PTHREAD_ATTR_FLAG_DETACHED      0x00000001
#define ANDROID_PTHREAD_ATTR_FLAG_USER_STACK    0x00000002

typedef struct
{
    uint32_t flags;
    void * stack_base;
    size_t stack_size;
    size_t guard_size;
    int32_t sched_policy;
    int32_t sched_priority;
} android_pthread_attr_t;

typedef int android_pthread_key_t;
typedef long android_pthread_t;
typedef volatile int  android_pthread_once_t;

int android_pthread_attr_init(android_pthread_attr_t * attr);

int android_pthread_attr_destroy(android_pthread_attr_t * attr);

int android_pthread_attr_setdetachstate(android_pthread_attr_t * attr, int state);

int android_pthread_attr_setschedparam(android_pthread_attr_t * attr, struct android_sched_param const * param);

int android_pthread_attr_setstacksize(android_pthread_attr_t * attr, size_t stack_size);
 
int android_pthread_create(android_pthread_t *thread, android_pthread_attr_t const * attr, void *(*start_routine)(void *), void * arg);

void *android_pthread_getspecific(android_pthread_key_t key);
 
int android_pthread_join(android_pthread_t thid, void ** ret_val);
 
int android_pthread_key_create(android_pthread_key_t *key, void (*destructor_function)(void *));

android_pthread_t android_pthread_self(void);

int android_pthread_setspecific(android_pthread_key_t key, const void *value);

int android_pthread_key_delete(android_pthread_key_t key);

int android_pthread_once(android_pthread_once_t *once_control, void (*init_routine)(void));

int android_pthread_equal(android_pthread_t t1, android_pthread_t t2);

int android_pthread_detach(android_pthread_t thread);

int android_pthread_setname_np(android_pthread_t thread, const char *name);