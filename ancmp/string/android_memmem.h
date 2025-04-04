#ifndef ANCMP_ANDROID_MEMMEM_H
#define ANCMP_ANDROID_MEMMEM_H

#include <stdlib.h>

void *android_memmem(const void *haystack, size_t n, const void *needle, size_t m);

#endif