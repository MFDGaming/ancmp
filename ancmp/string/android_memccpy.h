#ifndef ANCMP_ANDROID_MEMCCPY_H
#define ANCMP_ANDROID_MEMCCPY_H

#include <stdlib.h>

void *android_memccpy(void *dst, const void *src, int c, size_t n);

#endif