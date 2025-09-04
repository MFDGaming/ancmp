#include "android_fcntl.h"
#include <stdarg.h>
#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
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
                            ret |= ANDROID_O_WRONLY;
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
        HANDLE hFile = (HANDLE)_get_osfhandle(fd);
        LARGE_INTEGER len;
        LARGE_INTEGER start;
        OVERLAPPED ov;

        if (hFile == INVALID_HANDLE_VALUE) {
            va_end(args);
            return -1;
        }
        
        if (flock->l_whence == SEEK_SET) {
            start.QuadPart = flock->l_start;
        } else if (flock->l_whence == SEEK_CUR) {
            LARGE_INTEGER dist;
            LARGE_INTEGER cur_pos;

            dist.QuadPart = 0;
            cur_pos.LowPart = SetFilePointer(hFile, dist.LowPart, &dist.HighPart, FILE_CURRENT);
            cur_pos.HighPart = dist.HighPart;
    
            if (cur_pos.LowPart == -1 && GetLastError() != 0) {
                va_end(args);
                return -1;
            }
            start.QuadPart = cur_pos.QuadPart + flock->l_start;
        } else if (flock->l_whence == SEEK_END) {
            LARGE_INTEGER file_size;
            file_size.LowPart = GetFileSize(hFile, (LPDWORD)&file_size.HighPart);
            if(file_size.LowPart == -1 && GetLastError() != 0) {
                va_end(args);
                return -1;
            }
            start.QuadPart = file_size.QuadPart - flock->l_start;
        } else {
            va_end(args);
            return -1;
        }
        ZeroMemory(&ov, sizeof(OVERLAPPED));
        ov.Offset = start.LowPart;
        ov.OffsetHigh = start.HighPart;
        len.QuadPart = flock->l_len;
        if (op != ANDROID_F_GETLK) {
            if (flock->l_type == ANDROID_F_RDLCK || flock->l_type == ANDROID_F_WRLCK) {
                DWORD flags = (flock->l_type == ANDROID_F_WRLCK) ? LOCKFILE_EXCLUSIVE_LOCK : 0;
                if (op == ANDROID_F_SETLKW) {
                    if (!LockFileEx(hFile, flags, 0, len.LowPart, len.HighPart, &ov)) {
                        va_end(args);
                        return -1;
                    }
                } else {
                    if (!LockFileEx(hFile, flags | LOCKFILE_FAIL_IMMEDIATELY, 0, len.LowPart, len.HighPart, &ov)) {
                        va_end(args);
                        return -1;
                    }
                }
            } else if (flock->l_type == ANDROID_F_UNLCK) {
                if (!UnlockFileEx(hFile, 0, len.LowPart, len.HighPart, &ov)) {
                    va_end(args);
                    return -1;
                }
            }
        } else {
            DWORD flags = (flock->l_type == ANDROID_F_WRLCK) ? LOCKFILE_EXCLUSIVE_LOCK : 0;
            if (LockFileEx(hFile, flags | LOCKFILE_FAIL_IMMEDIATELY, 0, len.LowPart, len.HighPart, &ov)) {
                UnlockFileEx(hFile, 0, len.LowPart, len.HighPart, &ov);
                flock->l_type = ANDROID_F_UNLCK;
            } else {
                if (!(flock->l_type == ANDROID_F_RDLCK || flock->l_type == ANDROID_F_WRLCK)) {
                    flock->l_type = ANDROID_F_RDLCK;
                }
            }
        }
    }
    va_end(args);
    return ret;
}

int android_open(const char *pathname, int flags, ...) {
    DWORD attr = GetFileAttributes(pathname);
    va_list args;
    int fd;
    int real_flags = _O_BINARY;

    if ((attr != -1 && (attr & FILE_ATTRIBUTE_DIRECTORY))) {
        HANDLE dir = CreateFile(pathname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
        return _open_osfhandle((intptr_t)dir, _O_RDONLY);
    }
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

#ifdef _WIN32
    if ((!strcmp(pathname, "/dev/random")) || (!strcmp(pathname, "/dev/urandom"))) {
        pathname = "\\\\.\\pipe\\ancmp\\random";
    }
#endif

    va_start(args, flags);
    
    if (flags & ANDROID_O_CREAT) {
        unsigned short mode = va_arg(args, int);
        unsigned short real_mode = 0;
        if (mode & ANDROID_S_IRUSR) {
            real_mode |= _S_IREAD;
        }
        if (mode & ANDROID_S_IWUSR) {
            real_mode |= _S_IWRITE;
        }
        fd = _open(pathname, real_flags, real_mode);
    } else {
        fd = _open(pathname, real_flags);
    }
    va_end(args);
    return fd;
}

#endif
