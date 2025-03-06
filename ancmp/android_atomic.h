#pragma once

int android_atomic_cmpxchg(int old, int _new, volatile int *ptr);

int android_atomic_swap(int _new, volatile int *ptr);

int android_atomic_dec(volatile int *ptr);

int android_atomic_inc(volatile int *ptr);