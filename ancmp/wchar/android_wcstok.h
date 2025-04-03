#ifndef ANCMP_ANDROID_WCSTOK
#define ANCMP_ANDROID_WCSTOK

#include "android_wchar_internal.h"
#include <stdlib.h>

android_wchar_t *android_wcstok(android_wchar_t *s, const android_wchar_t *delim, android_wchar_t **last);

#endif