#ifndef ANCMP_ANDROID_SPRINT
#define ANCMP_ANDROID_SPRINT

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32

int android_vsnprintf(char *str, size_t n, const char *fmt, va_list ap);

int android_snprintf(char *str, size_t n, const char *fmt, ...);

int android_vsprintf(char *str, const char *fmt, va_list ap);

int android_sprintf(char *str, const char *fmt, ...);

#else

#define android_snprintf snprintf
#define android_vsnprintf vsnprintf
#define android_sprintf sprintf
#define android_vsprintf vsprintf

#endif

#endif