#include "android_atomic.h"

#ifdef _WIN32
void ANDROID_MEMBAR_FULL(void) {
#ifdef _MSC_VER
    __asm { lock add dword ptr [esp], 0 }
#else
    __asm__ volatile("lock addl $0, (%%esp)" ::: "memory");
#endif
}
#endif

int android_atomic_cmpxchg(int old, int _new, volatile int *addr) {
#ifdef _WIN32
    return InterlockedCompareExchange((LONG volatile *)addr, _new, old) != old;
#else
    return __sync_val_compare_and_swap(addr, old, _new) != old;
#endif
}

int android_atomic_swap(int _new, volatile int *addr) {
#ifdef _WIN32
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
