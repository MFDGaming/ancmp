#ifndef ANCMP_ANDROID_INET_NTOP
#define ANCMP_ANDROID_INET_NTOP

#include <stdlib.h>

const char *android_inet_ntop(int af, const void *src, char *dst, size_t size);

#endif