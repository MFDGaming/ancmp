#include "android_pthread_attr.h"
#include "android_errno.h"
#include <string.h>

const android_pthread_attr_t android_gDefaultPthreadAttr = {
    0,
    NULL,
    ANDROID_DEFAULT_STACKSIZE,
    ANDROID_PAGE_SIZE,
    ANDROID_SCHED_NORMAL,
    0
};

int android_pthread_attr_init(android_pthread_attr_t * attr)
{
    *attr = android_gDefaultPthreadAttr;
    return 0;
}

int android_pthread_attr_destroy(android_pthread_attr_t * attr)
{
    memset(attr, 0x42, sizeof(android_pthread_attr_t));
    return 0;
}

int android_pthread_attr_setdetachstate(android_pthread_attr_t * attr, int state)
{
    if (state == ANDROID_PTHREAD_CREATE_DETACHED) {
        attr->flags |= ANDROID_PTHREAD_ATTR_FLAG_DETACHED;
    } else if (state == ANDROID_PTHREAD_CREATE_JOINABLE) {
        attr->flags &= ~ANDROID_PTHREAD_ATTR_FLAG_DETACHED;
    } else {
        return ANDROID_EINVAL;
    }
    return 0;
}

int android_pthread_attr_getdetachstate(android_pthread_attr_t const * attr, int * state)
{
    *state = (attr->flags & ANDROID_PTHREAD_ATTR_FLAG_DETACHED)
           ? ANDROID_PTHREAD_CREATE_DETACHED
           : ANDROID_PTHREAD_CREATE_JOINABLE;
    return 0;
}

int android_pthread_attr_setschedpolicy(android_pthread_attr_t * attr, int policy)
{
    attr->sched_policy = policy;
    return 0;
}

int android_pthread_attr_getschedpolicy(android_pthread_attr_t const * attr, int * policy)
{
    *policy = attr->sched_policy;
    return 0;
}

int android_pthread_attr_setschedparam(android_pthread_attr_t * attr, struct android_sched_param const * param)
{
    attr->sched_priority = param->sched_priority;
    return 0;
}

int android_pthread_attr_getschedparam(android_pthread_attr_t const * attr, struct android_sched_param * param)
{
    param->sched_priority = attr->sched_priority;
    return 0;
}

int android_pthread_attr_setstacksize(android_pthread_attr_t * attr, size_t stack_size)
{
    if ((stack_size & (ANDROID_PAGE_SIZE - 1) || stack_size < ANDROID_PTHREAD_STACK_MIN)) {
        return ANDROID_EINVAL;
    }
    attr->stack_size = stack_size;
    return 0;
}

int android_pthread_attr_getstacksize(android_pthread_attr_t const * attr, size_t * stack_size)
{
    *stack_size = attr->stack_size;
    return 0;
}

int android_pthread_attr_setstackaddr(android_pthread_attr_t * attr, void * stack_addr)
{
#if 1
    /* It's not clear if this is setting the top or bottom of the stack, so don't handle it for now. */
    return ANDROID_ENOSYS;
#else
    if ((uint32_t)stack_addr & (PAGE_SIZE - 1)) {
        return EINVAL;
    }
    attr->stack_base = stack_addr;
    return 0;
#endif
}

int android_pthread_attr_getstackaddr(android_pthread_attr_t const * attr, void ** stack_addr)
{
#ifdef _WIN32
    return ANDROID_ENOSYS;
#else
    *stack_addr = (char*)attr->stack_base + attr->stack_size;
    return 0;
#endif
}

int android_pthread_attr_setstack(android_pthread_attr_t * attr, void * stack_base, size_t stack_size)
{
#ifdef _WIN32
    return ANDROID_ENOSYS;
#else
    if ((stack_size & (ANDROID_PAGE_SIZE - 1) || stack_size < ANDROID_PTHREAD_STACK_MIN)) {
        return ANDROID_EINVAL;
    }
    if ((uint32_t)stack_base & (ANDROID_PAGE_SIZE - 1)) {
        return ANDROID_EINVAL;
    }
    attr->stack_base = stack_base;
    attr->stack_size = stack_size;
    return 0;
#endif
}

int android_pthread_attr_getstack(android_pthread_attr_t const * attr, void ** stack_base, size_t * stack_size)
{
#ifdef _WIN32
    if (stack_base != NULL) {
        return ANDROID_ENOTSUP;
    }
#else
    *stack_base = attr->stack_base;
#endif
    *stack_size = attr->stack_size;
    return 0;
}

int android_pthread_attr_setguardsize(android_pthread_attr_t * attr, size_t guard_size)
{
#ifdef _WIN32
    return ANDROID_ENOSYS;
#else
    if (guard_size & (ANDROID_PAGE_SIZE - 1) || guard_size < ANDROID_PAGE_SIZE) {
        return ANDROID_EINVAL;
    }

    attr->guard_size = guard_size;
    return 0;
#endif
}

int android_pthread_attr_getguardsize(android_pthread_attr_t const * attr, size_t * guard_size)
{
#ifdef _WIN32
    return ANDROID_ENOSYS;
#else
    *guard_size = attr->guard_size;
    return 0;
#endif
}

int android_pthread_attr_setscope(android_pthread_attr_t *attr, int  scope)
{
    if (scope == ANDROID_PTHREAD_SCOPE_SYSTEM)
        return 0;
    if (scope == ANDROID_PTHREAD_SCOPE_PROCESS)
        return ANDROID_ENOTSUP;

    return ANDROID_EINVAL;
}

int android_pthread_attr_getscope(android_pthread_attr_t const *attr)
{
    return ANDROID_PTHREAD_SCOPE_SYSTEM;
}
