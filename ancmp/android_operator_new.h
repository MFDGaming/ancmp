#ifndef ANCMP_ANDROID_OPERATOR_NEW
#define ANCMP_ANDROID_OPERATOR_NEW

#include <stdlib.h>

void *android_operator_new(size_t size);

void *android_operator_new_arr(size_t size);

void android_operator_delete(void *ptr);

void android_operator_delete_arr(void *ptr);

void *android_operator_new_nothrow(size_t size, void *nothrow);

void *android_operator_new_arr_nothrow(size_t size, void *nothrow);

void android_operator_delete_nothrow(void *ptr, void *nothrow);

void android_operator_delete_arr_nothrow(void *ptr, void *nothrow);

#endif
