#include "android_mmap.h"
#include "posix_funcs.h"

#ifdef _WIN32

#include <windows.h>

#define PROT_TO_WINPROT(x)           (((x & ANDROID_PROT_EXEC) && (x & ANDROID_PROT_WRITE)) ? PAGE_EXECUTE_READWRITE : \
                                        (((x & ANDROID_PROT_EXEC) && (x & ANDROID_PROT_READ)) ? PAGE_EXECUTE_READ : \
                                            (((x & ANDROID_PROT_WRITE)) ? PAGE_READWRITE : \
                                                (((x & ANDROID_PROT_READ)) ? PAGE_READONLY : PAGE_NOACCESS \
                                            ) \
                                        ) \
                                    ))

void *android_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
    int overwrite = (flags & ANDROID_MAP_FIXED) != 0;
    void *ret = VirtualAlloc(overwrite ? addr : NULL, length, MEM_COMMIT | MEM_RESERVE, PROT_TO_WINPROT(prot));
    if (!ret) {
        return ANDROID_MAP_FAILED;
    }
    if (!(flags & ANDROID_MAP_ANONYMOUS)) {
        if (pread(fd, ret, length, offset) == -1) {
            VirtualFree(ret, length, MEM_RELEASE);
            return ANDROID_MAP_FAILED;
        }
    }
    return ret;
}

int android_munmap(void *addr, size_t length) {
    return VirtualFree(addr, length, MEM_RELEASE) ? 0 : (int)ANDROID_MAP_FAILED;
}

int android_mprotect(void *addr, size_t length, int prot) {
    DWORD old_protect;
    return VirtualProtect(addr, length, PROT_TO_WINPROT(prot), &old_protect) ? 0 : (int)ANDROID_MAP_FAILED;
}

#endif
