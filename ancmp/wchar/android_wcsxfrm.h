#ifndef ANCMP_ANDROID_WCSXFRM
#define ANCMP_ANDROID_WCSXFRM

#include "android_wchar_internal.h"
#include <stdlib.h>

size_t android_wcsxfrm(android_wchar_t *dest, const android_wchar_t *src, size_t len);

#endif