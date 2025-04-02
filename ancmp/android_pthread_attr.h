#ifndef ANCMP_ANDROID_PTHREAD_ATTR_H
#define ANCMP_ANDROID_PTHREAD_ATTR_H

#include <stdint.h>
#include <stdlib.h>
#include "android_sched.h"

#define ANDROID_PAGE_SIZE 4096

#define ANDROID_PTHREAD_STACK_MIN (2 * ANDROID_PAGE_SIZE)

#define ANDROID_PTHREAD_CREATE_DETACHED  0x00000001
#define ANDROID_PTHREAD_CREATE_JOINABLE  0x00000000

#define ANDROID_PTHREAD_ATTR_FLAG_DETACHED      0x00000001
#define ANDROID_PTHREAD_ATTR_FLAG_USER_STACK    0x00000002

#define ANDROID_PTHREAD_SCOPE_SYSTEM     0
#define ANDROID_PTHREAD_SCOPE_PROCESS    1

#define ANDROID_DEFAULT_STACKSIZE (1024 * 1024)

typedef struct {
    uint32_t flags;
    void * stack_base;
    size_t stack_size;
    size_t guard_size;
    int32_t sched_policy;
    int32_t sched_priority;
} android_pthread_attr_t;

extern const android_pthread_attr_t android_gDefaultPthreadAttr;

int android_pthread_attr_init(android_pthread_attr_t * attr);

int android_pthread_attr_destroy(android_pthread_attr_t * attr);

int android_pthread_attr_setdetachstate(android_pthread_attr_t * attr, int state);

int android_pthread_attr_getdetachstate(android_pthread_attr_t const * attr, int * state);

int android_pthread_attr_setschedpolicy(android_pthread_attr_t * attr, int policy);

int android_pthread_attr_getschedpolicy(android_pthread_attr_t const * attr, int * policy);

int android_pthread_attr_setschedparam(android_pthread_attr_t * attr, struct android_sched_param const * param);

int android_pthread_attr_getschedparam(android_pthread_attr_t const * attr, struct android_sched_param * param);

int android_pthread_attr_setstacksize(android_pthread_attr_t * attr, size_t stack_size);

int android_pthread_attr_getstacksize(android_pthread_attr_t const * attr, size_t * stack_size);

int android_pthread_attr_setstackaddr(android_pthread_attr_t * attr, void * stack_addr);

int android_pthread_attr_getstackaddr(android_pthread_attr_t const * attr, void ** stack_addr);

int android_pthread_attr_setstack(android_pthread_attr_t * attr, void * stack_base, size_t stack_size);

int android_pthread_attr_getstack(android_pthread_attr_t const * attr, void ** stack_base, size_t * stack_size);

int android_pthread_attr_setguardsize(android_pthread_attr_t * attr, size_t guard_size);

int android_pthread_attr_getguardsize(android_pthread_attr_t const * attr, size_t * guard_size);

int android_pthread_attr_setscope(android_pthread_attr_t *attr, int  scope);

int android_pthread_attr_getscope(android_pthread_attr_t const *attr);

#endif
