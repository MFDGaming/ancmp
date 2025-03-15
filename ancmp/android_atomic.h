#pragma once

#ifdef _WIN32
#include <windows.h>

#define ANDROID_MEMBAR_FULL MemoryBarrier
#else
#define ANDROID_MEMBAR_FULL __sync_synchronize
#endif

int android_atomic_cmpxchg(int old, int _new, volatile int *addr);

int android_atomic_swap(int _new, volatile int *addr);

int android_atomic_dec(volatile int *addr);