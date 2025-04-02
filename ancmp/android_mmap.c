#include "android_mmap.h"
#include "posix_funcs.h"
#include "android_memmap.h"

#ifdef _WIN32

void *android_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
    int overwrite = (flags & ANDROID_MAP_FIXED) != 0;
    void *ret = memmap_alloc(addr, length, overwrite);
    if (!ret) {
        return ANDROID_MAP_FAILED;
    }
    if (!(flags & ANDROID_MAP_ANONYMOUS)) {
        if (pread(fd, ret, length, offset) == -1) {
            if (!overwrite) {
                memmap_dealloc(ret, length);
            }
            return ANDROID_MAP_FAILED;
        }
    }
    return ret;
}

int android_munmap(void *addr, size_t length) {
    return memmap_dealloc(addr, length) ? 0 : (int)ANDROID_MAP_FAILED;
}

#endif
