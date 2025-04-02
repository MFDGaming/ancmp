#include "android_futex.h"

#ifdef _WIN32

#include <windows.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

typedef struct android_futex_lock {
    volatile void *addr;
    HANDLE semaphore;
    struct android_futex_lock *next;
    struct android_futex_lock *prev;
    volatile long ref_count;
} android_futex_lock_t;

static HANDLE android_futex_mtx = NULL;
static android_futex_lock_t *futex_locks_first = NULL;
static android_futex_lock_t *futex_locks_last = NULL;

android_futex_lock_t *android_futex_append_key(volatile void *addr) {
    android_futex_lock_t *current_lock = (android_futex_lock_t *)malloc(sizeof(android_futex_lock_t));
    if (current_lock == NULL) {
        return NULL;
    }
    current_lock->semaphore = CreateSemaphore(NULL, INT_MAX - 1, INT_MAX - 1, NULL);
    if (current_lock->semaphore == INVALID_HANDLE_VALUE) {
        free(current_lock);
        return NULL;
    }
    current_lock->addr = addr;
    current_lock->next = NULL;
    current_lock->ref_count = 0;
    if (futex_locks_first == NULL) {
        futex_locks_first = futex_locks_last = current_lock;
        current_lock->prev = NULL;
    } else {
        futex_locks_last->next = current_lock;
        current_lock->prev = futex_locks_last;
        futex_locks_last = current_lock;
    }
    return current_lock;
}

android_futex_lock_t *android_futex_get_lock(volatile void *addr, BOOL create, BOOL do_inc) {
    android_futex_lock_t *current_lock;

    WaitForSingleObject(android_futex_mtx, INFINITE);
    for (current_lock = futex_locks_first; current_lock != NULL; current_lock = current_lock->next) {
        if (current_lock->addr == addr) {
            if (do_inc) {
                InterlockedIncrement(&current_lock->ref_count);
            }
            ReleaseMutex(android_futex_mtx);
            return current_lock;
        }
    }
    current_lock = NULL;
    if (create) {
        current_lock = android_futex_append_key(addr);
        if (current_lock && do_inc) {
            InterlockedIncrement(&current_lock->ref_count);
        }
    }
    ReleaseMutex(android_futex_mtx);
    return current_lock;
}

void android_futex_cleanup_lock(android_futex_lock_t *lock, BOOL do_dec) {
    WaitForSingleObject(android_futex_mtx, INFINITE);
    if (lock != NULL) {
        if (do_dec) {
            InterlockedDecrement(&lock->ref_count);
        }
        if (InterlockedCompareExchange(&lock->ref_count, 0, 0) == 0) {
            if (lock->semaphore != INVALID_HANDLE_VALUE) {
                CloseHandle(lock->semaphore);
            }
            if (lock->prev == NULL && lock->next == NULL) {
                futex_locks_first = futex_locks_last = NULL;
            } else if (lock->prev == NULL) {
                futex_locks_first = lock->next;
                futex_locks_first->prev = NULL;
            } else if (lock->next == NULL) {
                futex_locks_last = lock->prev;
                futex_locks_last->next = NULL;
            } else {
                lock->prev->next = lock->next;
                lock->next->prev = lock->prev;
            }
            free(lock);
        }
    }
    ReleaseMutex(android_futex_mtx);
}

int android_futex_init(void) {
    android_futex_mtx = CreateMutex(NULL, FALSE, NULL);
    if (android_futex_mtx == NULL) {
        return 0;
    }
    return 1;
}

int android_futex_wake_ex(volatile void *ftx, int pshared, int val) {
    android_futex_lock_t *lock = android_futex_get_lock(ftx, FALSE, FALSE);
    int woke = 0;

    if (lock == NULL) {
        return -1;
    }
    if (lock->semaphore == INVALID_HANDLE_VALUE) {
        return -1;
    }
    if (val == INT_MAX) {
        LONG prev = 0;
        while (prev < (INT_MAX - 1)) {
            if (!ReleaseSemaphore(lock->semaphore, 1, &prev)) {
                break;
            }
            ++woke;
        }
    } else if (val >= 0) {
        LONG prev = 0;
        int i;

        for (i = 0; i < val; ++i) {
            if (!ReleaseSemaphore(lock->semaphore, 1, &prev) || prev >= (INT_MAX - 1)) {
                break;
            }
            ++woke;
        }
    } else {
        woke = -1;
    }
    return woke;
}

int android_futex_wait_ex(volatile void *ftx, int pshared, int val, const android_timespec_t *timeout) {
    android_futex_lock_t *lock;
    DWORD wait_time = INFINITE;

    if (InterlockedCompareExchange((long *)ftx, val, val) != val) {
        errno = EAGAIN;
        return -1;
    }
    lock = android_futex_get_lock(ftx, TRUE, TRUE);
    if (lock == NULL) {
        return -1;
    }
    if (lock->semaphore == INVALID_HANDLE_VALUE) {
        return -1;
    }
    if (timeout) {
        wait_time = (DWORD)((timeout->tv_sec * 1000) + (timeout->tv_nsec / 1000000));
    }
    WaitForSingleObject(lock->semaphore, wait_time);
    android_futex_cleanup_lock(lock, TRUE);
    return 0;
}

int android_futex_wait(volatile void *ftx, int val, const android_timespec_t *timeout) {
    return android_futex_wait_ex(ftx, 0, val, timeout);
}

int android_futex_wake(volatile void *ftx, int count) {
    return android_futex_wake_ex(ftx, 0, count);
}

#else

#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

/* Futex syscall wrapper */
static int futex(volatile void *addr, int op, int val, const android_timespec_t *timeout, volatile void *addr2, int val3) {
    return syscall(SYS_futex, addr, op, val, timeout, addr2, val3);
}

/* Wake `val` threads (with process-shared support) */
int android_futex_wake_ex(volatile void *ftx, int pshared, int val) {
    int op = pshared ? FUTEX_WAKE | FUTEX_PRIVATE_FLAG : FUTEX_WAKE;
    return futex(ftx, op, val, NULL, NULL, 0);
}

/* Wait with timeout (with process-shared support) */
int android_futex_wait_ex(volatile void *ftx, int pshared, int val, const android_timespec_t *timeout) {
    int op = pshared ? FUTEX_WAIT | FUTEX_PRIVATE_FLAG : FUTEX_WAIT;
    return futex(ftx, op, val, timeout, NULL, 0);
}

/* Standard futex wait (calls `__futex_wait_ex` with `pshared=0`) */
int android_futex_wait(volatile void *ftx, int val, const android_timespec_t *timeout) {
    return android_futex_wait_ex(ftx, 0, val, timeout);
}

/* Standard futex wake (calls `__futex_wake_ex` with `pshared=0`) */
int android_futex_wake(volatile void *ftx, int count) {
    return android_futex_wake_ex(ftx, 0, count);
}

#endif
