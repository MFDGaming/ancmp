#ifndef ANCMP_ANDROID_ATOMIC_H
#define ANCMP_ANDROID_ATOMIC_H

#ifdef _WIN32
#include <windows.h>
void ANDROID_MEMBAR_FULL(void);
#else
#define ANDROID_MEMBAR_FULL __sync_synchronize
#endif

int android_atomic_cmpxchg(int old, int _new, volatile int *addr);

int android_atomic_swap(int _new, volatile int *addr);

int android_atomic_dec(volatile int *addr);

#endif
