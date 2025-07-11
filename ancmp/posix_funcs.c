#include "posix_funcs.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>

long pread(int fd, void *buf, size_t count, off_t offset) {
    long retval;
    long pos = lseek(fd, 0, SEEK_CUR);
    if (pos == -1) {
        return -1;
    }
    if (lseek(fd, offset, SEEK_SET) == -1) {
        return -1;
    }
    retval = read(fd, buf, count);
    if (lseek(fd, pos, SEEK_SET) == -1) {
        puts("pread failed to reset position");
        abort();
    }
    return retval;
}
#endif
