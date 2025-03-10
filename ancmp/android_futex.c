#include "android_futex.h"

#ifdef _WIN32

#include <windows.h>

int android_futex_wake_ex(volatile void *ftx, int pshared, int val) {
    if (val == 1) {
        WakeByAddressSingle((PVOID)ftx);
    } else if (val > 1) {
        WakeByAddressAll((PVOID)ftx);
    }
    return val;
}

int android_futex_wait_ex(volatile void *ftx, int pshared, int val, const struct timespec *timeout) {
    DWORD timeout_ms = INFINITE;
    if (timeout) {
        timeout_ms = (DWORD)(timeout->tv_sec * 1000 + timeout->tv_nsec / 1000000);
    }

    int current_val = val;
    if (!WaitOnAddress((PVOID)ftx, &current_val, sizeof(int), timeout_ms)) {
        if (GetLastError() == ERROR_TIMEOUT) {
            return -1;
        }
        return -1;
    }
    return 0;
}

int android_futex_wait(volatile void *ftx, int val, const struct timespec *timeout) {
    return android_futex_wait_ex(ftx, 0, val, timeout);
}

int android_futex_wake(volatile void *ftx, int count) {
    return android_futex_wake_ex(ftx, 0, count);
}

#else

#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

// Futex syscall wrapper
static int futex(volatile void *addr, int op, int val, const struct timespec *timeout, volatile void *addr2, int val3) {
    return syscall(SYS_futex, addr, op, val, timeout, addr2, val3);
}

// Wake `val` threads (with process-shared support)
int android_futex_wake_ex(volatile void *ftx, int pshared, int val) {
    int op = pshared ? FUTEX_WAKE | FUTEX_PRIVATE_FLAG : FUTEX_WAKE;
    return futex(ftx, op, val, NULL, NULL, 0);
}

// Wait with timeout (with process-shared support)
int android_futex_wait_ex(volatile void *ftx, int pshared, int val, const struct timespec *timeout) {
    int op = pshared ? FUTEX_WAIT | FUTEX_PRIVATE_FLAG : FUTEX_WAIT;
    return futex(ftx, op, val, timeout, NULL, 0);
}

// Standard futex wait (calls `__futex_wait_ex` with `pshared=0`)
int android_futex_wait(volatile void *ftx, int val, const struct timespec *timeout) {
    return android_futex_wait_ex(ftx, 0, val, timeout);
}

// Standard futex wake (calls `__futex_wake_ex` with `pshared=0`)
int android_futex_wake(volatile void *ftx, int count) {
    return android_futex_wake_ex(ftx, 0, count);
}

#endif