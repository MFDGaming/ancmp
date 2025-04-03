#ifndef ANCMP_ANDROID_WCSNCPY
#define ANCMP_ANDROID_WCSNCPY

#include "android_wchar_internal.h"
#include <stdlib.h>

android_wchar_t *android_wcsncpy(android_wchar_t *dst, const android_wchar_t *src, size_t n);

#endif