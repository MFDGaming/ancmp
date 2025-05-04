#ifndef ANCMP_ANDROID_STRFTIME
#define ANCMP_ANDROID_STRFTIME

#include <time.h>

size_t android_strftime(char *s, size_t maxsize, const char *format, const struct tm *t);

#endif
