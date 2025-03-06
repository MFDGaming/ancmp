#include <stdatomic.h>
#include "android_atomic.h"

int android_atomic_cmpxchg(int old, int _new, volatile int *ptr) {
    atomic_int *atomic_ptr = (atomic_int *)ptr;
    return !atomic_compare_exchange_strong(atomic_ptr, &old, _new);
}

int android_atomic_swap(int _new, volatile int *ptr) {
    atomic_int *atomic_ptr = (atomic_int *)ptr;
    return atomic_exchange(atomic_ptr, _new);
}

int android_atomic_dec(volatile int *ptr) {
    atomic_int *atomic_ptr = (atomic_int *)ptr;
    return atomic_fetch_sub(atomic_ptr, 1);
}

int android_atomic_inc(volatile int *ptr) {
    atomic_int *atomic_ptr = (atomic_int *)ptr;
    return atomic_fetch_add(atomic_ptr, 1);
}