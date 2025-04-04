#ifndef ANCMP_ANDROID_WCSLCPY_H
#define ANCMP_ANDROID_WCSLCPY_H

#include "android_wchar_internal.h"
#include <stdlib.h>

size_t android_wcslcpy(android_wchar_t *dst, const android_wchar_t *src, size_t siz);

#endif