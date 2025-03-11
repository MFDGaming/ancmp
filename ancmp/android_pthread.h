#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "android_sched.h"

#define ANDROID_PTHREAD_CREATE_DETACHED  0x00000001
#define ANDROID_PTHREAD_CREATE_JOINABLE  0x00000000

#define ANDROID_PTHREAD_ONCE_INIT    0

#define ANDROID_PTHREAD_PROCESS_PRIVATE  0
#define ANDROID_PTHREAD_PROCESS_SHARED   1

#define ANDROID_PTHREAD_SCOPE_SYSTEM     0
#define ANDROID_PTHREAD_SCOPE_PROCESS    1

#define PTHREAD_COND_INITIALIZER  {0}
#define PTHREAD_MUTEX_INITIALIZER {0}

#define ANDROID_PTHREAD_ATTR_FLAG_DETACHED      0x00000001
#define ANDROID_PTHREAD_ATTR_FLAG_USER_STACK    0x00000002

#define ANDROID_COND_SHARED_MASK        0x0001
#define ANDROID_COND_COUNTER_INCREMENT  0x0002
#define ANDROID_COND_COUNTER_MASK       (~ANDROID_COND_SHARED_MASK)

#define ANDROID_COND_IS_SHARED(c)  (((c)->value & ANDROID_COND_SHARED_MASK) != 0)

#define  ANDROID_MUTEX_OWNER(m)  (((m)->value >> 16) & 0xffff)
#define  ANDROID_MUTEX_COUNTER(m) (((m)->value >> 2) & 0xfff)

#define  ANDROID_MUTEX_TYPE_MASK       0xc000
#define  ANDROID_MUTEX_TYPE_NORMAL     0x0000
#define  ANDROID_MUTEX_TYPE_RECURSIVE  0x4000
#define  ANDROID_MUTEX_TYPE_ERRORCHECK 0x8000

#define  ANDROID_MUTEX_COUNTER_SHIFT  2
#define  ANDROID_MUTEX_COUNTER_MASK   0x1ffc
#define  ANDROID_MUTEX_SHARED_MASK    0x2000

#define  ANDROID_MUTEXATTR_TYPE_MASK   0x000f
#define  ANDROID_MUTEXATTR_SHARED_MASK 0x0010

#define  ANDROID_PTHREAD_MUTEX_INITIALIZER             {0}
#define  ANDROID_PTHREAD_RECURSIVE_MUTEX_INITIALIZER   {0x4000}
#define  ANDROID_PTHREAD_ERRORCHECK_MUTEX_INITIALIZER  {0x8000}

enum {
    ANDROID_PTHREAD_MUTEX_NORMAL = 0,
    ANDROID_PTHREAD_MUTEX_RECURSIVE = 1,
    ANDROID_PTHREAD_MUTEX_ERRORCHECK = 2,

    ANDROID_PTHREAD_MUTEX_ERRORCHECK_NP = ANDROID_PTHREAD_MUTEX_ERRORCHECK,
    ANDROID_PTHREAD_MUTEX_RECURSIVE_NP  = ANDROID_PTHREAD_MUTEX_RECURSIVE,

    ANDROID_PTHREAD_MUTEX_DEFAULT = ANDROID_PTHREAD_MUTEX_NORMAL
};

typedef struct
{
    uint32_t flags;
    void * stack_base;
    size_t stack_size;
    size_t guard_size;
    int32_t sched_policy;
    int32_t sched_priority;
} android_pthread_attr_t;

typedef struct
{
    int volatile value;
} android_pthread_cond_t;

typedef struct
{
    int volatile value;
} android_pthread_mutex_t;

typedef long android_pthread_condattr_t;
typedef long android_pthread_mutexattr_t;

typedef int android_pthread_key_t;
typedef long android_pthread_t;
typedef volatile int  android_pthread_once_t;

int android_pthread_attr_init(android_pthread_attr_t * attr);

int android_pthread_attr_destroy(android_pthread_attr_t * attr);

int android_pthread_attr_setdetachstate(android_pthread_attr_t * attr, int state);

int android_pthread_attr_setschedparam(android_pthread_attr_t * attr, struct android_sched_param const * param);

int android_pthread_attr_setstacksize(android_pthread_attr_t * attr, size_t stack_size);

int android_pthread_cond_broadcast(android_pthread_cond_t *cond);

int android_pthread_cond_destroy(android_pthread_cond_t *cond);

int android_pthread_cond_init(android_pthread_cond_t *cond, const android_pthread_condattr_t *attr);

int android_pthread_mutex_trylock(android_pthread_mutex_t *mutex);

int android_pthread_cond_timedwait(android_pthread_cond_t *cond, android_pthread_mutex_t * mutex, const struct timespec *abstime);

int android_pthread_cond_wait(android_pthread_cond_t *cond, android_pthread_mutex_t *mutex);
 
int android_pthread_create(android_pthread_t *thread, android_pthread_attr_t const * attr, void *(*start_routine)(void *), void * arg);

void *android_pthread_getspecific(android_pthread_key_t key);
 
int android_pthread_join(android_pthread_t thid, void ** ret_val);
 
int android_pthread_key_create(android_pthread_key_t *key, void (*destructor_function)(void *));

int android_pthread_mutexattr_destroy(android_pthread_mutexattr_t *attr);
 
int android_pthread_mutexattr_init(android_pthread_mutexattr_t *attr);

int android_pthread_mutex_destroy(android_pthread_mutex_t *mutex);

int android_pthread_mutex_init(android_pthread_mutex_t *mutex, const android_pthread_mutexattr_t *attr);

int android_pthread_mutex_lock(android_pthread_mutex_t *mutex);

int android_pthread_mutex_unlock(android_pthread_mutex_t *mutex);

android_pthread_t android_pthread_self(void);

int android_pthread_setspecific(android_pthread_key_t key, const void *value);

int android_pthread_key_delete(android_pthread_key_t key);

int android_pthread_once(android_pthread_once_t *once_control, void (*init_routine)(void));

int android_pthread_equal(android_pthread_t t1, android_pthread_t t2);

int android_pthread_cond_signal(android_pthread_cond_t *cond);

int android_pthread_detach(android_pthread_t thread);

int android_pthread_setname_np(android_pthread_t thread, const char *name);