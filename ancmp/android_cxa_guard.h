#ifndef ANCMP_ANDROID_CXA_GUARD
#define ANCMP_ANDROID_CXA_GUARD

#include "linker.h"

#ifdef ANDROID_ARM_LINKER
typedef union {
    int volatile state;
    int32_t aligner;
} android_guard_t;
#else
#ifdef ANDROID_X86_LINKER
typedef union {
    int volatile state;
    int64_t aligner;
} android_guard_t;
#else
#error Unsupported architecture. Only ARM and x86 are presently supported.
#endif
#endif

int android_cxa_guard_acquire(android_guard_t *gv);

void android_cxa_guard_release(android_guard_t *gv);

void android_cxa_guard_abort(android_guard_t *gv);

#endif