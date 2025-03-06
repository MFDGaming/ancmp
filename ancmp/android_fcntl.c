#include "android_fcntl.h"
#include <stdarg.h>
#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#include <io.h>
#include <fcntl.h>
#include "android_stat.h"

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
    }
    return ret;
}

int android_open(const char *pathname, int flags, ...) {
    int real_flags = 0;
    if (flags & ANDROID_O_RDONLY) {
        real_flags |= O_RDONLY;
    }
    if (flags & ANDROID_O_WRONLY) {
        real_flags |= O_WRONLY;
    }
    if (flags & ANDROID_O_RDWR) {
        real_flags |= O_RDWR;
    }
    if (flags & ANDROID_O_CREAT) {
        real_flags |= O_CREAT;
    }
    if (flags & ANDROID_O_EXCL) {
        real_flags |= O_EXCL;
    }
    if (flags & ANDROID_O_TRUNC) {
        real_flags |= O_TRUNC;
    }
    if (flags & ANDROID_O_APPEND) {
        real_flags |= O_APPEND;
    }
    if (flags & ANDROID_O_ACCMODE) {
        real_flags |= O_ACCMODE;
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

    return fd;
}

#endif