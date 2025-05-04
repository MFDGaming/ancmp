#ifndef ANCMP_ANDROID_STRTO
#define ANCMP_ANDROID_STRTO

#include "ancmp_stdint.h"

uintmax_t android_strtoumax(const char *nptr, char **endptr, int base);

uint64_t android_strtoull(const char *nptr, char **endptr, int base);

#endif
