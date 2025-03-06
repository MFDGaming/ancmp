#include "android_futex.h"

#ifdef _WIN32

#include <windows.h>

typedef long NTSTATUS;

// NTAPI function pointers
typedef NTSTATUS (NTAPI *NtWaitForKeyedEvent_t)(HANDLE, PVOID, BOOLEAN, PLARGE_INTEGER);
typedef NTSTATUS (NTAPI *NtReleaseKeyedEvent_t)(HANDLE, PVOID, BOOLEAN, PLARGE_INTEGER);

static NtWaitForKeyedEvent_t NtWaitForKeyedEvent;
static NtReleaseKeyedEvent_t NtReleaseKeyedEvent;

static void futex_init() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    NtWaitForKeyedEvent = (NtWaitForKeyedEvent_t)GetProcAddress(ntdll, "NtWaitForKeyedEvent");
    NtReleaseKeyedEvent = (NtReleaseKeyedEvent_t)GetProcAddress(ntdll, "NtReleaseKeyedEvent");
}

static void timespec_to_largeint(const struct timespec *timeout, LARGE_INTEGER *li) {
    if (timeout) {
        li->QuadPart = -(timeout->tv_sec * 10000000LL + timeout->tv_nsec / 100LL);
    }
}

int android_futex_wake_ex(volatile void *ftx, int pshared, int val) {
    if (!NtReleaseKeyedEvent) futex_init();
    
    for (int i = 0; i < val; i++) {
        NTSTATUS status = NtReleaseKeyedEvent(pshared ? GetCurrentProcess() : NULL, (PVOID)ftx, FALSE, NULL);
        if (status != 0) return -1;
    }
    return 0;
}

int android_futex_wait_ex(volatile void *ftx, int pshared, int val, const struct timespec *timeout) {
    if (!NtWaitForKeyedEvent) futex_init();
    
    LARGE_INTEGER li_timeout, *pli = NULL;
    if (timeout) {
        timespec_to_largeint(timeout, &li_timeout);
        pli = &li_timeout;
    }
    
    return NtWaitForKeyedEvent(pshared ? GetCurrentProcess() : NULL, (PVOID)ftx, FALSE, pli) == 0 ? 0 : -1;
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