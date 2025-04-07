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
#if defined(_WIN32) && defined(_MSC_VER)
    return InterlockedDecrement((LONG volatile *)addr) + 1;
#else
    return __sync_fetch_and_sub(addr, 1);
#endif
}

int android_atomic_inc(volatile int *addr) {
#if defined(_WIN32) && defined(_MSC_VER)
    return InterlockedIncrement((LONG volatile *)addr) - 1;
#else
    return __sync_fetch_and_add(addr, 1);
#endif
}
