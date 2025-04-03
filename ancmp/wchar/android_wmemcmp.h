#ifndef ANCMP_ANDROID_WMEMCMP
#define ANCMP_ANDROID_WMEMCMP

#include "android_wchar_internal.h"
#include <stdlib.h>

int android_wmemcmp(const android_wchar_t *s1, const android_wchar_t *s2, size_t n);

#endif