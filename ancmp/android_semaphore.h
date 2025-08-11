#ifndef ANCMP_ANDROID_SEMAPHORE_H
#define ANCMP_ANDROID_SEMAPHORE_H

#include "android_time.h"

#define ANDROID_SEM_FAILED NULL

typedef struct {
    volatile unsigned int count;
} android_sem_t;

int android_sem_init(android_sem_t *sem, int pshared, unsigned int value);

int android_sem_destroy(android_sem_t *sem);

android_sem_t *android_sem_open(const char *name, int oflag, ...);

int android_sem_close(android_sem_t *sem);

int android_sem_unlink(const char *name);

int android_sem_wait(android_sem_t *sem);

int android_sem_timedwait(android_sem_t *sem, const android_timespec_t *abs_timeout);

int android_sem_post(android_sem_t *sem);

int android_sem_trywait(android_sem_t *sem);

int android_sem_getvalue(android_sem_t *sem, int *sval);

#endif