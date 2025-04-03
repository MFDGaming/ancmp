#ifndef ANCMP_ANDROID_WMEMCPY
#define ANCMP_ANDROID_WMEMCPY

#include "android_wchar_internal.h"
#include <stdlib.h>

android_wchar_t *android_wmemcpy(android_wchar_t *d, const android_wchar_t *s, size_t n);

#endif