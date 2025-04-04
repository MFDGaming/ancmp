#ifndef ANCMP_ANDROID_STRTOK_H
#define ANCMP_ANDROID_STRTOK_H

#include <stdlib.h>

char *android_strtok(char *s, const char *delim);

char *android_strtok_r(char *s, const char *delim, char **last);

#endif
