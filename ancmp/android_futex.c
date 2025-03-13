#include "android_futex.h"

#ifdef _WIN32

#include <windows.h>
#include <winternl.h>
#include <ntstatus.h>
#include <stdio.h>

typedef NTSTATUS (NTAPI *NtWaitForKeyedEvent_t)(HANDLE, PVOID, BOOLEAN, PLARGE_INTEGER);
typedef NTSTATUS (NTAPI *NtReleaseKeyedEvent_t)(HANDLE, PVOID, BOOLEAN, PLARGE_INTEGER);
typedef NTSTATUS (NTAPI *NtCreateKeyedEvent_t)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, ULONG);
typedef NTSTATUS (NTAPI *NtOpenKeyedEvent_t)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES);

static NtWaitForKeyedEvent_t NtWaitForKeyedEvent;
static NtReleaseKeyedEvent_t NtReleaseKeyedEvent;
static NtCreateKeyedEvent_t NtCreateKeyedEvent;
static NtOpenKeyedEvent_t NtOpenKeyedEvent;

HANDLE android_futex_event = NULL;
BOOL android_futex_inited = FALSE;
HANDLE android_futex_mtx = NULL;

typedef struct android_futex_key {
    unsigned int key;
    struct android_futex_key *next;
} android_futex_key_t;

unsigned int current_key = 2;
android_futex_key_t *released_keys = NULL;
android_futex_key_t *released_keys_last = NULL;
BOOL no_keys_left = FALSE;

void android_futex_append_key(unsigned int key) {
    WaitForSingleObject(android_futex_mtx, INFINITE);
    android_futex_key_t *current_key = (android_futex_key_t *)malloc(sizeof(android_futex_key_t));
    current_key->key = key;
    current_key->next = NULL;
    if (released_keys == NULL) {
        released_keys = released_keys_last = current_key;
    } else {
        released_keys_last->next = current_key;
    }
    ReleaseMutex(android_futex_mtx);
}

BOOL android_futex_get_key(unsigned int *key) {
    WaitForSingleObject(android_futex_mtx, INFINITE);
    if (released_keys != NULL) {
        android_futex_key_t *old_key = released_keys;
        *key = released_keys->key;
        released_keys = released_keys->next;
        free(old_key);
    } else {
        if (!no_keys_left) {
            if (current_key == 0xfffffffe) {
                no_keys_left = TRUE;
            }
            *key = current_key;
            current_key += 2;
        } else {
            ReleaseMutex(android_futex_mtx);
            return FALSE;
        }
    }
    ReleaseMutex(android_futex_mtx);
    return TRUE;
}

int android_futex_init() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    NtWaitForKeyedEvent = (NtWaitForKeyedEvent_t)GetProcAddress(ntdll, "NtWaitForKeyedEvent");
    NtReleaseKeyedEvent = (NtReleaseKeyedEvent_t)GetProcAddress(ntdll, "NtReleaseKeyedEvent");
    NtCreateKeyedEvent = (NtCreateKeyedEvent_t)GetProcAddress(ntdll, "NtCreateKeyedEvent");
    NtOpenKeyedEvent = (NtOpenKeyedEvent_t)GetProcAddress(ntdll, "NtOpenKeyedEvent");
    android_futex_mtx = CreateMutex(NULL, FALSE, NULL);
    if (!(NtWaitForKeyedEvent && NtReleaseKeyedEvent && NtCreateKeyedEvent && NtOpenKeyedEvent)) {
        return 0;
    }
    if (NtCreateKeyedEvent(&android_futex_event, EVENT_ALL_ACCESS, NULL, 0) != STATUS_SUCCESS) {
        return 0;
    }
    if (!android_futex_mtx) {
        return 0;
    }
    return 1;
}

static void timespec_to_largeint(const struct timespec *timeout, LARGE_INTEGER *li) {
    li->QuadPart = (-1LL) * ((timeout->tv_sec * 10000000LL) + (timeout->tv_nsec / 100LL));
}

int android_futex_wake_ex(volatile void *ftx, int pshared, int val) {
    unsigned int h = InterlockedCompareExchange((LONG *)ftx, 0, 0);
    LARGE_INTEGER li = {
        .QuadPart = -1000LL
    };
    if (h) {
        NtReleaseKeyedEvent(android_futex_event, (void *)h, FALSE, &li);
        return val;
    }
    return 0;
}

int android_futex_wait_ex(volatile void *ftx, int pshared, int val, const struct timespec *timeout) {
    if (InterlockedCompareExchange(ftx, val, val) != val) {
        return -1;
    }
    unsigned int event;
    if (!android_futex_get_key(&event)) {
        return -1;
    }
    unsigned int h = InterlockedCompareExchange(ftx, event, 0);
    if (h) {
        android_futex_append_key(event);
    } else {
        h = event;
    }
    LARGE_INTEGER li;
    LARGE_INTEGER *lip = NULL;
    if (timeout) {
        timespec_to_largeint(timeout, &li);
        lip = &li;
    }

    NtWaitForKeyedEvent(android_futex_event, (void *)h, TRUE, lip);

    android_futex_append_key(h);
    InterlockedCompareExchange(ftx, 0, h);
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