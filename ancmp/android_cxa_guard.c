#include "android_cxa_guard.h"
#include "android_atomic.h"
#include "android_futex.h"
#include <limits.h>
#include <stdio.h>

#ifdef ANDROID_ARM_LINKER
const static int ready = 0x1;
const static int pending = 0x2;
const static int waiting = 0x6;
#else
#ifdef ANDROID_X86_LINKER
const static int ready = 0x1;
const static int pending = 0x100;
const static int waiting = 0x10000;
#else
#error Unsupported architecture. Only ARM and x86 are presently supported.
#endif
#endif

int android_cxa_guard_acquire(android_guard_t *gv) {
retry:
    if (android_atomic_cmpxchg(0, pending, &gv->state) == 0) {
        ANDROID_MEMBAR_FULL();
        return 1;
    }
    android_atomic_cmpxchg(pending, waiting, &gv->state);
    android_futex_wait(&gv->state, waiting, NULL);
    if (gv->state != ready)
        goto retry;
    ANDROID_MEMBAR_FULL();
    return 0;
}

void android_cxa_guard_release(android_guard_t *gv) {
    ANDROID_MEMBAR_FULL();
    if (android_atomic_cmpxchg(pending, ready, &gv->state) == 0) {
        return;
    }
    gv->state = ready;
    android_futex_wake(&gv->state, INT_MAX);
}

void android_cxa_guard_abort(android_guard_t *gv) {
    ANDROID_MEMBAR_FULL();
    gv->state = 0;
    android_futex_wake(&gv->state, INT_MAX);
}
