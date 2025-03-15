#include "android_atomic.h"

int android_atomic_cmpxchg(int old, int _new, volatile int *ptr) {
#ifdef _WIN32
    return InterlockedCompareExchange((LONG volatile *)ptr, _new, old) != old;
#else
    return __sync_val_compare_and_swap(atomic_ptr, old, _new) != old;
#endif
}

int android_atomic_swap(int _new, volatile int *ptr) {
#ifdef _WIN32
    return InterlockedExchange((LONG volatile *)ptr, _new);
#else
    return __sync_lock_test_and_set(ptr, _new);
#endif
}

int android_atomic_dec(volatile int *ptr) {
#ifdef _WIN32
    return InterlockedDecrement((LONG volatile *)ptr);
#else
    return __sync_sub_and_fetch(ptr, 1);
#endif
}

int android_atomic_inc(volatile int *ptr) {
#ifdef _WIN32
    return InterlockedIncrement((LONG volatile *)ptr);
#else
    return __sync_add_and_fetch(ptr, 1);
#endif
}