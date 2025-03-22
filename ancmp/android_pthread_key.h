#pragma once

typedef int android_pthread_key_t;

int android_pthread_key_create(android_pthread_key_t *key, void (*destructor_function)(void *));

int android_pthread_key_delete(android_pthread_key_t key);

void *android_pthread_getspecific(android_pthread_key_t key);

int android_pthread_setspecific(android_pthread_key_t key, const void *ptr);