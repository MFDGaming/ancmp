#ifndef ANCMP_POSIX_FUNCS_H
#define ANCMP_POSIX_FUNCS_H

#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#ifdef _WIN32
long pread(int fd, void *buf, size_t count, off_t offset);
#endif

#endif
