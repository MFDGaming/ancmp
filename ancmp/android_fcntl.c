#include "android_fcntl.h"
#include <stdarg.h>
#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#include <io.h>
#include <fcntl.h>
#include "android_stat.h"
#include <sys/stat.h>

int is_socket(int fd) {
    int protocol_info;
    int len = sizeof(protocol_info);
    return getsockopt(fd, SOL_SOCKET, SO_TYPE, (char *)&protocol_info, &len) != -1;
}

static int check_nonblocksock(SOCKET sock) {
    u_long mode;
    int result = ioctlsocket(sock, FIONBIO, &mode);
    if (result != 0) {
        //printf("Error in ioctlsocket: %d\n", WSAGetLastError());
        return 0;
    }
    return (mode != 0);
}

int android_fcntl(int fd, int op, ...) {
    int ret = 0;
    int is_s = is_socket(fd);
    va_list args;
    va_start(args, op);
    if (op == ANDROID_F_GETFL) {
        if (is_s) {
            if (check_nonblocksock(fd)) {
                ret |= ANDROID_O_NONBLOCK;
            }
        } else {
            HANDLE hFile = (HANDLE)_get_osfhandle(fd);
            if (hFile != INVALID_HANDLE_VALUE) {
                DWORD dwDesiredAccess;
                if (GetHandleInformation(hFile, &dwDesiredAccess)) {
                    if (dwDesiredAccess & GENERIC_READ && dwDesiredAccess & GENERIC_WRITE) {
                        ret |= ANDROID_O_RDWR;
                    } else {
                        if (dwDesiredAccess & GENERIC_WRITE) {
                            ret |= ANDROID_O_RDONLY;
                        }
                        if (dwDesiredAccess & GENERIC_READ) {
                            ret |= ANDROID_O_RDONLY;
                        }
                    }
                }
            }
        }
    } else if (op == ANDROID_F_SETFL) {
        int flags = va_arg(args, int);
        if (flags & ANDROID_O_NONBLOCK) {
            if (is_s) {
                unsigned long nonBlocking = 1;
                ioctlsocket(fd, FIONBIO, &nonBlocking);
            } else {
                return -1;
            }
        } else {
            return -1;
        }
    } else if (op == ANDROID_F_SETLK || op == ANDROID_F_SETLKW || op == ANDROID_F_GETLK) {
        android_flock_t *flock = va_arg(args, android_flock_t *);
        HANDLE handle = (HANDLE)_get_osfhandle(fd);
        if (handle == INVALID_HANDLE_VALUE) {
            va_end(args);
            return -1;
        }
        LARGE_INTEGER len = {.QuadPart = flock->l_len};
        LARGE_INTEGER start;
        long cur_pos = lseek(fd, 0, SEEK_CUR);
        if (flock->l_whence == SEEK_SET) {
            start.QuadPart = flock->l_start;
        } else if (flock->l_whence == SEEK_CUR) {
            if (cur_pos == -1) {
                va_end(args);
                return -1;
            }
            start.QuadPart = flock->l_start + cur_pos;
        } else if (flock->l_whence == SEEK_END) {
            if (cur_pos == -1) {
                va_end(args);
                return -1;
            }
            long end = lseek(fd, 0, SEEK_END);
            if (end == -1) {
                lseek(fd, cur_pos, SEEK_SET);
                va_end(args);
                return -1;
            }
            lseek(fd, cur_pos, SEEK_SET);
            start.QuadPart = end - flock->l_start;
        } else {
            va_end(args);
            return -1;
        }
        OVERLAPPED ov = {
            .Offset = start.LowPart,
            .OffsetHigh = start.HighPart
        };
        if (op != ANDROID_F_GETLK) {
            if (flock->l_type == ANDROID_F_RDLCK || flock->l_type == ANDROID_F_WRLCK) {
                if (!LockFileEx(handle, (op == ANDROID_F_SETLKW) ? 0 : LOCKFILE_FAIL_IMMEDIATELY, 0, len.LowPart, len.HighPart, &ov)) {
                    va_end(args);
                    return -1;
                }
            } else if (flock->l_type == ANDROID_F_UNLCK) {
                if (!UnlockFileEx(handle, 0, len.LowPart, len.HighPart, &ov)) {
                    va_end(args);
                    return -1;
                }
            }
        } else {
            if (!LockFileEx(handle, LOCKFILE_FAIL_IMMEDIATELY, 0, len.LowPart, len.HighPart, &ov)) {
                if (!(flock->l_type == ANDROID_F_RDLCK || flock->l_type == ANDROID_F_WRLCK)) {
                    flock->l_type = ANDROID_F_WRLCK;
                }
            } else {
                UnlockFileEx(handle, 0, len.LowPart, len.HighPart, &ov);
                flock->l_type = ANDROID_F_UNLCK;
            }
        }
    }
    va_end(args);
    return ret;
}

int android_open(const char *pathname, int flags, ...) {
    int real_flags = 0;
    if (flags & ANDROID_O_APPEND) {
        real_flags |= _O_APPEND;
    }
    if (flags & ANDROID_O_CREAT) {
        real_flags |= _O_CREAT;
    }
    if (flags & ANDROID_O_EXCL) {
        real_flags |= _O_EXCL;
    }
    if (flags & ANDROID_O_RDONLY) {
        real_flags |= _O_RDONLY;
    }
    if (flags & ANDROID_O_RDWR) {
        real_flags |= _O_RDWR;
    }
    if (flags & ANDROID_O_TRUNC) {
        real_flags |= _O_TRUNC;
    }
    if (flags & ANDROID_O_WRONLY) {
        real_flags |= _O_WRONLY;
    }

    va_list args;
    va_start(args, flags);
    
    int fd;
    if (real_flags & O_CREAT) {
        unsigned short mode = va_arg(args, int);
        fd = open(pathname, real_flags, s_to_native(mode));
    } else {
        fd = open(pathname, real_flags);
    }
    if (fd != -1) {
        printf("\x1b[32mSuccessfully ran open() for %s\x1b[0m\n", pathname);
    } else {
        printf("\x1b[31mFailed to run open() for %s due to %d\x1b[0m\n", pathname, errno);
    }

    return fd;
}

#endif