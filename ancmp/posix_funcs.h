#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#ifdef _WIN32
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1

typedef int clockid_t;

ssize_t pread(int fd, void *buf, size_t count, off_t offset);

int clock_gettime(clockid_t clk_id, struct timespec *tp);
#endif

size_t bsd_strlcpy(char *dst, const char *src, size_t siz);

char *bsd_strsep(char **stringp, const char *delim);