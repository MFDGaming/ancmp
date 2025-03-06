#include <stdint.h>
#include <time.h>

int android_futex_wake_ex(volatile void *ftx, int pshared, int val);

int android_futex_wait_ex(volatile void *ftx, int pshared, int val, const struct timespec *timeout);

int android_futex_wait(volatile void *ftx, int val, const struct timespec *timeout);

int android_futex_wake(volatile void *ftx, int count);