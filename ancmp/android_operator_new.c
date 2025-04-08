#include "android_operator_new.h"
#include <stdio.h>

void *android_operator_new(size_t size) {
    void* p = malloc(size);
    if (p == NULL) {
        puts("Failed to allocate memory");
        abort();
    }
    return p;
}

void *android_operator_new_arr(size_t size) {
    void *p = malloc(size);
    if (p == NULL) {
        puts("Failed to allocate memory");
        abort();
    }
    return p;
}

void android_operator_delete(void *ptr) {
    free(ptr);
}

void android_operator_delete_arr(void *ptr) {
    free(ptr);
}

void *android_operator_new_nothrow(size_t size, void *nothrow) {
    return malloc(size);
}

void *android_operator_new_arr_nothrow(size_t size, void *nothrow) {
    return malloc(size);
}

void android_operator_delete_nothrow(void *ptr, void *nothrow) {
    free(ptr);
}

void android_operator_delete_arr_nothrow(void *ptr, void *nothrow) {
    free(ptr);
}
