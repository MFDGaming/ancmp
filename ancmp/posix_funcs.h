#include <stdlib.h>
#include <sys/types.h>

#ifdef _WIN32
ssize_t pread(int fd, void *buf, size_t count, off_t offset);
#endif

size_t bsd_strlcpy(char *dst, const char *src, size_t siz);

char *bsd_strsep(char **stringp, const char *delim);