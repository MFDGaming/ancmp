#ifndef ANCMP_ANDROID_FUTEX_H
#define ANCMP_ANDROID_FUTEX_H

#include <stdint.h>
#include <time.h>
#include "android_time.h"

#ifdef _WIN32
int android_futex_init(void);
#endif

int android_futex_wake_ex(volatile void *ftx, int pshared, int val);

int android_futex_wait_ex(volatile void *ftx, int pshared, int val, const android_timespec_t *timeout);

int android_futex_wait(volatile void *ftx, int val, const android_timespec_t *timeout);

int android_futex_wake(volatile void *ftx, int count);

#endif
