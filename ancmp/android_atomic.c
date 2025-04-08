#include "android_atomic.h"

#if defined(_WIN32) && defined(_MSC_VER)
void ANDROID_MEMBAR_FULL(void) {
    __asm { lock add dword ptr [esp], 0 }
}
#endif

int android_atomic_cmpxchg(int old, int _new, volatile int *addr) {
#if defined(_WIN32) && defined(_MSC_VER)
    return InterlockedCompareExchange((LONG volatile *)addr, _new, old) != old;
#else
    return __sync_val_compare_and_swap(addr, old, _new) != old;
#endif
}

int android_atomic_swap(int _new, volatile int *addr) {
#if defined(_WIN32) && defined(_MSC_VER)
    return InterlockedExchange((LONG volatile *)addr, _new);
#else
    return __sync_lock_test_and_set(addr, _new);
#endif
}

int android_atomic_dec(volatile int *addr) {
    int old;
    do {
        old = *addr;
    } while (android_atomic_cmpxchg(old, old-1, addr));
    return old;
}

int android_atomic_inc(volatile int *addr) {
    int old;
    do {
        old = *addr;
    } while (android_atomic_cmpxchg(old, old+1, addr));
    return old;
}

int android_atomic_or(int _mask, volatile int *addr) {
    int old;
    do {
        old = *addr;
    } while (android_atomic_cmpxchg(old, old | _mask, addr));
    return old;
}

int android_atomic_and(int _mask, volatile int *addr) {
    int old;
    do {
        old = *addr;
    } while (android_atomic_cmpxchg(old, old & _mask, addr));
    return old;
}
