#include "hooks.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <float.h>
#include <stdio.h>
#include <signal.h>
#include <wchar.h>
#include <locale.h>
#include <wctype.h>
#include <errno.h>
#include <time.h>
#include <wchar.h>
#include "android_pthread.h"
#include "android_math.h"
#include "android_io.h"
#include "android_ctypes.h"
#include "android_stat.h"
#include "android_socket.h"
#include "android_dirent.h"
#include "android_posix_types.h"
#include "android_fcntl.h"
#include "android_ioctl.h"
#include "android_mmap.h"
#include "android_dlfcn.h"
#include "linker.h"
#include "android_sysconf.h"
#include "posix_funcs.h"
#include "android_cxa.h"
#include "android_strcasecmp.h"
#include "android_errno.h"
#include "android_time.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <direct.h>
#else
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/select.h>
#include <netdb.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/uio.h>
#endif

typedef struct {
    char *name;
    void *addr;
} hook_t;


int android_stack_chk_guard_location[4] = {0xDEADCAFE, 0xDAC0FFEE, 0x8BADF00D, 0xBEEFFACE};
void *android_stack_chk_guard = (void *)android_stack_chk_guard_location;

#ifdef _WIN32

int android_mkdir(const char *pathname, int mode) {
    BOOL ret = CreateDirectory(pathname, NULL);
    if (!ret) {
        printf("\x1b[31mFailed to mkdir %s due to %lu\x1b[0m\n", pathname, GetLastError());
    }
    return ret ? 0 : -1;
}

int android_pipe(int fds[2]) {
    HANDLE handles[2];
    if (!CreatePipe(&handles[0], &handles[1], NULL, 0)) {
        return -1;
    }
    fds[0] = _open_osfhandle((intptr_t)handles[0], _O_RDONLY);
    fds[1] = _open_osfhandle((intptr_t)handles[1], _O_WRONLY);
    if (fds[0] == -1 || fds[1] == -1) {
        CloseHandle(handles[0]);
        CloseHandle(handles[1]);
    }
    return 0;
}

int android_getsockname(int sockfd, struct sockaddr *addr, android_socklen_t *addrlen) {
    union {
        struct sockaddr_in ipv4;
        struct sockaddr_in6 ipv6;
    } local_addr;
    int len = sizeof(local_addr);
    if (getsockname(sockfd, (struct sockaddr *)&local_addr, &len) == 0) {
        local_addr.ipv4.sin_family = af_to_android(local_addr.ipv4.sin_family);
        if (local_addr.ipv4.sin_family == ANDROID_AF_INET) {
            memcpy(addr, &local_addr.ipv4, (*addrlen < sizeof(struct sockaddr_in)) ? *addrlen : sizeof(struct sockaddr_in));
            *addrlen = sizeof(struct sockaddr_in);
        } else if (local_addr.ipv4.sin_family == ANDROID_AF_INET6) {
            memcpy(addr, &local_addr.ipv6, (*addrlen < sizeof(struct sockaddr_in6)) ? *addrlen : sizeof(struct sockaddr_in6));
            *addrlen = sizeof(struct sockaddr_in6);
        } else {
            return -1;
        }
        return 0;
        
    }
    return -1;
}

int android_select(int nfds, android_fd_set_t *readfds, android_fd_set_t *writefds, android_fd_set_t *exceptfds, struct timeval *timeout) {
    fd_set native_readfds, native_writefds, native_exceptfds;
    fd_set *native_preadfds = NULL, *native_pwritefds = NULL, *native_pexceptfds = NULL;
    if (readfds) {
        fd_set_to_native(readfds, &native_readfds);
        native_preadfds = &native_readfds;
    }
    if (writefds) {
        fd_set_to_native(writefds, &native_writefds);
        native_pwritefds = &native_writefds;
    }
    if (exceptfds) {
        fd_set_to_native(exceptfds, &native_exceptfds);
        native_pexceptfds = &native_exceptfds;
    }
    return select(nfds, native_preadfds, native_pwritefds, native_pexceptfds, timeout);
}

int android_gethostname(char *name, size_t size) {
    DWORD len = size - 1;
    if (GetComputerNameEx(ComputerNameDnsHostname, name, &len)) {
        name[size - 1] = '\0';
        return 0;
    }
    memset(name, 0, size);
    return -1;
}

char *android_inet_ntoa(uint32_t addr) {
    struct in_addr real_addr;
    memcpy(&real_addr, &addr, sizeof(uint32_t));
    return inet_ntoa(real_addr);
}

uint32_t android_inet_addr(char *addr) {
    puts("android_inet_addr");
    return inet_addr(addr);
}

void android_div(div_t *ret, int numerator, int denominator) {
    puts("android_div");
    ret->quot = numerator/denominator;
    ret->rem = numerator%denominator;
}

static uint64_t seed48[3];

static const uint64_t multiplier = 0x5DEECE66D;
static const uint64_t addend = 0xB;
static const uint64_t mask = ((uint64_t)1 << 48) - 1;

void android_srand48(long seedval) {
    puts("android_srand48");
    seed48[0] = (seedval ^ multiplier) & mask;
    seed48[1] = 0x330E;
    seed48[2] = 0x0;
}

long android_lrand48(void) {
    puts("android_lrand48");
    seed48[0] = (multiplier * seed48[0] + addend) & mask;
    return (long)(seed48[0] >> 16) & 0x7FFFFFFF;
}

FLOAT_ABI_FIX double android_drand48(void) {
    puts("android_drand48");
    return (double)android_lrand48() / (1 << 31);
}

long android_syscall(long number, ...) {
    puts("android_syscall");
    return -1;
}

typedef struct {
    void  *iov_base;    /* Starting address */
    size_t iov_len;     /* Number of bytes to transfer */
} android_iovec_t;

long android_writev(int fd, const android_iovec_t *iov, int iovcnt) {
    int cnt = 0;
    int i;
    for (i = 0; i < iovcnt; ++i) {
        long len = android_write(fd, iov[i].iov_base, iov[i].iov_len);
        if (len == -1) {
            return -1;
        }
        cnt += len;
    }
    return cnt;
}

typedef struct {
    int fd;
    short events;
    short revents;
} android_pollfd_t;

typedef unsigned int  android_nfds_t;

int android_poll(android_pollfd_t *fds, android_nfds_t nfds, long timeout) {
    puts("android_poll");
    return -1;
}

int android_sigaction(int signum, void *act, void *oldact) {
    puts("android_sigaction");
    return -1;
}

int android_sched_yield(void) {
    puts("android_sched_yield");
    return SwitchToThread() ? 0 : -1;
}

int android_fsync(int fd) {
    HANDLE hFile = (HANDLE)_get_osfhandle(fd);
    if (hFile == INVALID_HANDLE_VALUE) {
        puts("android_fsync failed");
        return -1;
    }
    if (!FlushFileBuffers(hFile)) {
        puts("android_fsync failed");
        return -1;
    }
    return 0;
}

int android_fdatasync(int fd) {
    HANDLE hFile = (HANDLE)_get_osfhandle(fd);
    if (hFile == INVALID_HANDLE_VALUE) {
        puts("android_fdatasync failed");
        return -1;
    }
    if (!FlushFileBuffers(hFile)) {
        puts("android_fdatasync failed");
        return -1;
    }
    return 0;
}

int android_geteuid(void) {
    puts("android_geteuid");
    return 0;
}

int android_getpid(void) {
    return GetCurrentProcessId();
}

int android_remove(const char *pathname) {
    DWORD attr = GetFileAttributes(pathname);
    BOOL ret = FALSE;
    if (attr != INVALID_FILE_ATTRIBUTES) {
        if (attr & FILE_ATTRIBUTE_DIRECTORY) {
            ret = RemoveDirectory(pathname);
        } else {
            ret = DeleteFile(pathname);
        }
    }
    if (!ret) {
        printf("\x1b[31mFailed to remove %s due to %lu\x1b[0m\n", pathname, GetLastError());
    }
    return ret ? 0 : -1;
}

int android_rename(const char *oldpath, const char *newpath) {
    BOOL ret = MoveFileEx(oldpath, newpath, MOVEFILE_REPLACE_EXISTING);
    if (!ret ) {
        printf("\x1b[31mFailed to rename %s to %s due to %lu\x1b[0m\n", oldpath, newpath, GetLastError());
    }
    return ret ? 0 : -1;
}

int android_unlink(const char *pathname) {
    BOOL ret = DeleteFile(pathname);
    if (!ret) {
        printf("\x1b[31mFailed to unlink %s due to %lu\x1b[0m\n", pathname, GetLastError());
    }
    return ret ? 0 : -1;
}

int android_shutdown(int sockfd, int how) {
    return shutdown(sockfd, how);
}

int android_vsnprintf(char *str, size_t n, const char *fmt, va_list ap) {
	char dummy;
    va_list ap_copy;
    int ret = 0;
	if (n > INT_MAX) {
		n = INT_MAX;
    }
	if (n == 0) {
		str = &dummy;
		n = 1;
	}
    va_copy(ap_copy, ap);
    ret = _vscprintf(fmt, ap_copy);
    va_end(ap_copy);
    _vsnprintf(str, n, fmt, ap);
    if ((ret + 1) > n) {
	    str[n - 1] = '\0';
    } else {
        str[ret] = '\0';
    }
	return ret;
}

int android_snprintf(char *str, size_t n, const char *fmt, ...) {
	va_list ap;
	int ret;
    va_start(ap, fmt);
    ret = android_vsnprintf(str, n, fmt, ap);
    va_end(ap);
	return ret;
}

int android_vsprintf(char *str, const char *fmt, va_list ap) {
    return android_vsnprintf(str, INT_MAX, fmt, ap);
}

int android_sprintf(char *str, const char *fmt, ...) {
    va_list ap;
	int ret;
    va_start(ap, fmt);
    ret = android_vsnprintf(str, INT_MAX, fmt, ap);
    va_end(ap);
	return ret;
}

#else
#define android_mkdir mkdir
#define android_pipe pipe
#define android_select select
#define android_gethostname gethostname
#define android_getsockname getsockname
#define android_div div
#define android_inet_ntoa inet_ntoa
#define android_inet_addr inet_addr
#define android_lrand48 lrand48
#define android_srand48 srand48
#define android_syscall syscall
#define android_writev writev
#define android_poll poll
#define android_sigaction sigaction
#define android_sched_yield sched_yield
#define android_fsync fsync
#define android_fdatasync fdatasync
#define android_geteuid geteuid
#define android_getpid getpid
#define android_remove remove
#define android_rename rename
#define android_unlink unlink
#define android_shutdown shutdown
#define android_snprintf snprintf
#define android_vsnprintf vsnprintf
#define android_sprintf sprintf
#define android_vsprintf vsprintf
#endif
#ifdef _WIN32
#if (_WIN32_WINNT >= 0x0501)
#define HAS_GETADDRINFO
#endif
#else
#define HAS_GETADDRINFO
#endif

typedef struct {
    char    *h_name;
    char    **h_aliases;
    int     h_addrtype;
    int     h_length;
    char    **h_addr_list;
} android_hostent_t;

static char h_name[256];
static char h_addr_list_storage[35][16];
static char h_aliases_storage[35][256];
static char *h_addr_list[35];
static char *h_aliases[35];

static android_hostent_t android_host = {
    h_name,
    h_aliases,
    ANDROID_AF_INET,
    4,
    h_addr_list
};

#ifdef HAS_GETADDRINFO
android_hostent_t *android_gethostbyname(const char *name) {
    struct addrinfo *info;
    
    if(getaddrinfo(name, NULL, NULL, &info) == 0) {
        int i;
        struct addrinfo *original_info = info;
        for (i = 0; (i < 34) && (info != NULL); (++i), (info = info->ai_next)) {
            for (;;) {
                if (info == NULL) {
                    break;
                }
                if (info->ai_family == AF_INET) {
                    break;
                }
                info = info->ai_next;
            }
            if (info == NULL) {
                break;
            }

            if (info->ai_canonname) {
                size_t cnl = strlen(info->ai_canonname) + 1;
                memcpy(h_aliases_storage[i], info->ai_canonname, cnl <= 256 ? cnl : 256);
                h_aliases_storage[i][255] = '\0';
            } else {
                h_aliases_storage[i][0] = '\0';
            }
            h_aliases[i] = h_aliases_storage[i];

            memcpy(h_addr_list_storage[i], info->ai_addr, info->ai_addrlen);
            h_addr_list[i] = h_addr_list_storage[i];
        }
        freeaddrinfo(original_info);
        h_addr_list[i] = NULL;
        h_aliases[i] = NULL;
        return &android_host;
    }
    return NULL;
}
#else
#define android_gethostbyname gethostbyname
#endif

FLOAT_ABI_FIX double android_strtod(const char *nptr, char **endptr) {
    return strtod(nptr, endptr);
}

void android_stack_chk_fail(void) {
    puts("__stack_chk_fail");
}

size_t android_strlen_chk(const char *s, size_t s_len) {
    size_t ret = strlen(s);
    if (ret >= s_len) {
        abort();
    }
    return ret;
}

FLOAT_ABI_FIX int android_isinff(float x) {
    return isinf(x);
}

FLOAT_ABI_FIX int android_isfinitef(float x) {
    return isfinite(x);
}
 
FLOAT_ABI_FIX int android_isfinite(double x) {
    return isfinite(x);
}

FLOAT_ABI_FIX int android_isnanf(float x) {\
    return isnan(x);
}

void android_assert2(const char *__file, int __line, const char *__function, const char *__msg) {
    printf("Assertion failed: %s\nFile: %s\nLine: %d\nFunction: %s\n", __msg, __file, __line, __function);
    abort();
}

static hook_t pthread_hooks[] = {
    {
        "pthread_attr_init",
        (void *)android_pthread_attr_init
    },
    {
        "pthread_attr_destroy",
        (void *)android_pthread_attr_destroy
    },
    {
        "pthread_attr_setdetachstate",
        (void *)android_pthread_attr_setdetachstate
    },
    {
        "pthread_attr_setschedparam",
        (void *)android_pthread_attr_setschedparam
    },
    {
        "pthread_attr_setstacksize",
        (void *)android_pthread_attr_setstacksize
    },
    {
        "pthread_cond_broadcast",
        (void *)android_pthread_cond_broadcast
    },
    {
        "pthread_cond_destroy",
        (void *)android_pthread_cond_destroy
    },
    {
        "pthread_cond_init",
        (void *)android_pthread_cond_init
    },
    {
        "pthread_mutex_trylock",
        (void *)android_pthread_mutex_trylock
    },
    {
        "pthread_cond_timedwait",
        (void *)android_pthread_cond_timedwait
    },
    {
        "pthread_cond_wait",
        (void *)android_pthread_cond_wait
    },
    {
        "pthread_create",
        (void *)android_pthread_create
    },
    {
        "pthread_getspecific",
        (void *)android_pthread_getspecific
    },
    {
        "pthread_join",
        (void *)android_pthread_join
    },
    {
        "pthread_key_create",
        (void *)android_pthread_key_create
    },
    {
        "pthread_mutexattr_destroy",
        (void *)android_pthread_mutexattr_destroy
    },
    {
        "pthread_mutexattr_init",
        (void *)android_pthread_mutexattr_init
    },
    {
        "pthread_mutex_destroy",
        (void *)android_pthread_mutex_destroy
    },
    {
        "pthread_mutex_init",
        (void *)android_pthread_mutex_init
    },
    {
        "pthread_mutex_lock",
        (void *)android_pthread_mutex_lock
    },
    {
        "pthread_mutex_unlock",
        (void *)android_pthread_mutex_unlock
    },
    {
        "pthread_self",
        (void *)android_pthread_self
    },
    {
        "pthread_setspecific",
        (void *)android_pthread_setspecific
    },
    {
        "pthread_key_delete",
        (void *)android_pthread_key_delete
    },
    {
        "pthread_once",
        (void *)android_pthread_once
    },
    {
        "pthread_equal",
        (void *)android_pthread_equal
    },
    {
        "pthread_cond_signal",
        (void *)android_pthread_cond_signal
    },
    {
        "pthread_detach",
        (void *)android_pthread_detach
    },
    {
        "pthread_setname_np",
        (void *)android_pthread_setname_np
    },
    {
        (char *)NULL,
        (void *)NULL
    }
};

static hook_t math_hooks[] = {
    {
        "atan2f",
        (void *)android_atan2f
    },
    {
        "ceilf",
        (void *)android_ceilf
    },
    {
        "cosf",
        (void *)android_cosf
    },
    {
        "floorf",
        (void *)android_floorf
    },
    {
        "fmodf",
        (void *)android_fmodf
    },
    {
        "logf",
        (void *)android_logf
    },
    {
        "powf",
        (void *)android_powf
    },
    {
        "sinf",
        (void *)android_sinf
    },
    {
        "sqrtf",
        (void *)android_sqrtf
    },
    {
        "atanf",
        (void *)android_atanf
    },
    {
        "fmod",
        (void *)android_fmod
    },
    {
        "floor",
        (void *)android_floor
    },
    {
        "ceil",
        (void *)android_ceil
    },
    {
        "sin",
        (void *)android_sin
    },
    {
        "cos",
        (void *)android_cos
    },
    {
        "sqrt",
        (void *)android_sqrt
    },
    {
        "pow",
        (void *)android_pow
    },
    {
        "atan2",
        (void *)android_atan2
    },
    {
        "atan",
        (void *)android_atan
    },
    {
        "modff",
        (void *)android_modff
    },
    {
        "modf",
        (void *)android_modf
    },
    {
        "ldexp",
        (void *)android_ldexp
    },
    {
        "ldexpf",
        (void *)android_ldexpf
    },
    {
        "tanf",
        (void *)android_tanf
    },
    {
        (char *)NULL,
        (void *)NULL
    }
};

static hook_t hooks[] = {
    {
        "__cxa_finalize",
        (void *)android_cxa_finalize
    },
    {
        "__cxa_atexit",
        (void *)android_cxa_atexit
    },
    {
        "__cxa_pure_virtual",
        (void *)android_cxa_pure_virtual
    },
    {
        "__stack_chk_fail",
        (void *)android_stack_chk_fail
    },
    {
        "__stack_chk_guard",
        (void *)&android_stack_chk_guard
    },
    {
        "__strlen_chk",
        (void *)android_strlen_chk
    },
    {
        "__isnanf",
        (void *)android_isnanf
    },
    {
        "__isinff",
        (void *)android_isinff
    },
    {
        "__isfinitef",
        (void *)android_isfinitef
    },
    {
        "__isfinite",
        (void *)android_isfinite
    },
    {
        "div",
        (void *)android_div
    },
    {
        "strcasecmp",
        (void *)android_strcasecmp
    },
    {
        "strerror",
        (void *)android_strerror
    },
    {
        "strncasecmp",
        (void *)android_strncasecmp
    },
    {
        "__errno",
        (void *)android_errno
    },
    {
        "toupper",
        (void *)android_toupper
    },
    {
        "tolower",
        (void *)android_tolower
    },
    {
        "fflush",
        (void *)android_fflush
    },
    {
        "fseek",
        (void *)android_fseek
    },
    {
        "fgetpos",
        (void *)android_fgetpos
    },
    {
        "fputs",
        (void *)android_fputs
    },
    {
        "inet_ntoa",
        (void *)android_inet_ntoa
    },
    {
        "inet_addr",
        (void *)android_inet_addr
    },
    {
        "fsetpos",
        (void *)android_fsetpos
    },
    {
        "ftell",
        (void *)android_ftell
    },
    {
        "fwrite",
        (void *)android_fwrite
    },
    {
        "fread",
        (void *)android_fread
    },
    {
        "ungetc",
        (void *)android_ungetc
    },
    {
        "fopen",
        (void *)android_fopen
    },
    {
        "fclose",
        (void *)android_fclose
    },
    {
        "putc",
        (void *)android_putc
    },
    {
        "setvbuf",
        (void *)android_setvbuf
    },
    {
        "rename",
        (void *)android_rename
    },
    {
        "__sF",
        (void *)&android_sf
    },
    {
        "_tolower_tab_",
        (void *)&android_tolower_tab
    },
    {
        "_ctype_",
        (void *)&android_ctype
    },
    {
        "gettimeofday",
        (void *)android_gettimeofday
    },
    {
        "fstat",
        (void *)android_fstat
    },
    {
        "closedir",
        (void *)android_closedir
    },
    {
        "opendir",
        (void *)android_opendir
    },
    {
        "readdir",
        (void *)android_readdir
    },
    {
        "remove",
        (void *)android_remove
    },
    {
        "mkdir",
        (void *)android_mkdir
    },
    {
        "pipe",
        (void *)android_pipe
    },
    {
        "sysconf",
        (void *)android_sysconf
    },
    {
        "usleep",
        (void *)android_usleep
    },
    {
        "gethostname",
        (void *)android_gethostname
    },
    {
        "listen",
        (void *)android_listen
    },
    {
        "send",
        (void *)android_send
    },
    {
        "sendto",
        (void *)android_sendto
    },
    {
        "recv",
        (void *)android_recv
    },
    {
        "recvfrom",
        (void *)android_recvfrom
    },
    {
        "getsockopt",
        (void *)android_getsockopt
    },
    {
        "setsockopt",
        (void *)android_setsockopt
    },
    {
        "socket",
        (void *)android_socket
    },
    {
        "accept",
        (void *)android_accept
    },
    {
        "bind",
        (void *)android_bind
    },
    {
        "connect",
        (void *)android_connect
    },
    {
        "gethostbyname",
        (void *)android_gethostbyname
    },
    {
        "getsockname",
        (void *)android_getsockname
    },
    {
        "shutdown",
        (void *)android_shutdown
    },
    {
        "select",
        (void *)android_select
    },
    {
        "fcntl",
        (void *)android_fcntl
    },
    {
        "ioctl",
        (void *)android_ioctl
    },
    {
        "mmap",
        (void *)android_mmap
    },
    {
        "munmap",
        (void *)android_munmap
    },
    {
        "close",
        (void *)android_close
    },
    {
        "read",
        (void *)android_read
    },
    {
        "write",
        (void *)android_write
    },
    {
        "open",
        (void *)android_open
    },
    {
        "getc",
        (void *)android_getc
    },
    {
        "__assert2",
        (void *)android_assert2
    },
    {
        "fprintf",
        (void *)android_fprintf
    },
    {
        "fscanf",
        (void *)android_fscanf
    },
    {
        "fgets",
        (void *)android_fgets
    },
    {
        "lrand48",
        (void *)android_lrand48
    },
    {
        "srand48",
        (void *)android_srand48
    },
    {
        "getpid",
        (void *)android_getpid
    },
    {
        "nanosleep",
        (void *)android_nanosleep
    },
    {
        "syscall",
        (void *)android_syscall
    },
    {
        "strtod",
        (void *)android_strtod
    },
    {
        "fputc",
        (void *)android_fputc
    },
    {
        "putwc",
        (void *)android_putwc
    },
    {
        "ungetwc",
        (void *)android_ungetwc
    },
    {
        "getwc",
        (void *)android_getwc
    },
    {
        "fdopen",
        (void *)android_fdopen
    },
    {
        "writev",
        (void *)android_writev
    },
    {
        "poll",
        (void *)android_poll
    },
    {
        "ferror",
        (void *)android_ferror
    },
    {
        "feof",
        (void *)android_feof
    },
    {
        "stat",
        (void *)android_stat
    },
    {
        "sigaction",
        (void *)android_sigaction
    },
    {
        "sched_yield",
        (void *)android_sched_yield
    },
    {
        "localtime_r",
        (void *)android_localtime_r
    },
    {
        "fsync",
        (void *)android_fsync
    },
    {
        "fdatasync",
        (void *)android_fdatasync
    },
    {
        "unlink",
        (void *)android_unlink
    },
    {
        "geteuid",
        (void *)android_geteuid
    },
    {
        "dlopen",
        (void *)android_dlopen
    },
    {
        "dlclose",
        (void *)android_dlclose
    },
    {
        "dlsym",
        (void *)android_dlsym
    },
    {
        "dladdr",
        (void *)android_dladdr
    },
    {
        "dlerror",
        (void *)android_dlerror
    },
#ifdef ANDROID_ARM_LINKER
    {
        "dl_iterate_phdr",
        (void *)android_dl_unwind_find_exidx
    },
#else
    {
        "dl_iterate_phdr",
        (void *)android_dl_iterate_phdr
    },
#endif
    {
        "vsnprintf",
        (void *)android_vsnprintf
    },
    {
        "snprintf",
        (void *)android_snprintf
    },
    {
        "vsprintf",
        (void *)android_vsprintf
    },
    {
        "sprintf",
        (void *)android_sprintf
    },
    {
        "clock_gettime",
        (void *)android_clock_gettime
    },
    {
        "ftime",
        (void *)android_ftime
    },
    {
        "pread",
        (void *)pread
    },
    {
        "bsd_signal",
        (void *)signal
    },
    {
        "malloc",
        (void *)malloc
    },
    {
        "realloc",
        (void *)realloc
    },
    {
        "free",
        (void *)free
    },
    {
        "exit",
        (void *)exit
    },
    {
        "abort",
        (void *)abort
    },
    {
        "memchr",
        (void *)memchr
    },
    {
        "memcmp",
        (void *)memcmp
    },
    {
        "memmove",
        (void *)memmove
    },
    {
        "memset",
        (void *)memset
    },
    {
        "wmemcpy",
        (void *)wmemcpy
    },
    {
        "memcpy",
        (void *)memcpy
    },
    {
        "wmemmove",
        (void *)wmemmove
    },
    {
        "wmemset",
        (void *)wmemset
    },
    {
        "isalpha",
        (void *)isalpha
    },
    {
        "iscntrl",
        (void *)iscntrl
    },
    {
        "islower",
        (void *)islower
    },
    {
        "isprint",
        (void *)isprint
    },
    {
        "ispunct",
        (void *)ispunct
    },
    {
        "isspace",
        (void *)isspace
    },
    {
        "isupper",
        (void *)isupper
    },
    {
        "isxdigit",
        (void *)isxdigit
    },
    {
        "iswalpha",
        (void *)iswalpha
    },
    {
        "iswcntrl",
        (void *)iswcntrl
    },
    {
        "iswdigit",
        (void *)iswdigit
    },
    {
        "iswlower",
        (void *)iswlower
    },
    {
        "iswprint",
        (void *)iswprint
    },
    {
        "iswpunct",
        (void *)iswpunct
    },
    {
        "iswspace",
        (void *)iswspace
    },
    {
        "iswupper",
        (void *)iswupper
    },
    {
        "iswxdigit",
        (void *)iswxdigit
    },
    {
        "strcat",
        (void *)strcat
    },
    {
        "strchr",
        (void *)strchr
    },
    {
        "strcmp",
        (void *)strcmp
    },
    {
        "strcpy",
        (void *)strcpy
    },
    {
        "strlen",
        (void *)strlen
    },
    {
        "strncpy",
        (void *)strncpy
    },
    {
        "strstr",
        (void *)strstr
    },
    {
        "strtok",
        (void *)strtok
    },
    {
        "strtoull",
        (void *)strtoull
    },
    {
        "wcscmp",
        (void *)wcscmp
    },
    {
        "wcslen",
        (void *)wcslen
    },
    {
        "wcsncpy",
        (void *)wcsncpy
    },
    {
        "setlocale",
        (void *)setlocale
    },
    {
        "towupper",
        (void *)towupper
    },
    {
        "towlower",
        (void *)towlower
    },
    {
        "putchar",
        (void *)putchar
    },
    {
        "printf",
        (void *)printf
    },
    {
        "sscanf",
        (void *)sscanf
    },
    {
        "puts",
        (void *)puts
    },
    {
        "time",
        (void *)time
    },
    {
        "mktime",
        (void *)mktime
    },
    {
        "access",
        (void *)access
    },
    {
        "atoi",
        (void *)atoi
    },
    {
        "calloc",
        (void *)calloc
    },
    {
        "strpbrk",
        (void *)strpbrk
    },
    {
        "gmtime",
        (void *)gmtime
    },
    {
        "wmemchr",
        (void *)wmemchr
    },
    {
        "wmemcmp",
        (void *)wmemcmp
    },
    {
        "wctype",
        (void *)wctype
    },
    {
        "iswctype",
        (void *)iswctype
    },
    {
        "wctob",
        (void *)wctob
    },
    {
        "btowc",
        (void *)btowc
    },
    {
        "wcrtomb",
        (void *)wcrtomb
    },
    {
        "mbrtowc",
        (void *)mbrtowc
    },
    {
        "strcoll",
        (void *)strcoll
    },
    {
        "strxfrm",
        (void *)strxfrm
    },
    {
        "wcscoll",
        (void *)wcscoll
    },
    {
        "wcsxfrm",
        (void *)wcsxfrm
    },
    {
        "strftime",
        (void *)strftime
    },
    {
        "wcsftime",
        (void *)wcsftime
    },
    {
        "strtol",
        (void *)strtol
    },
    {
        "raise",
        (void *)raise
    },
    {
        "rmdir",
        (void *)rmdir
    },
    {
        "perror",
        (void *)perror
    },
    {
        "strrchr",
        (void *)strrchr
    },
    {
        "getenv",
        (void *)getenv
    },
    {
        "lseek",
        (void *)lseek
    },
    {
        (char *)NULL,
        (void *)NULL
    }
};

static hook_t *custom_hooks = NULL;
static int custom_hooks_size = 0;
static int custom_hooks_arr_size = 0;

static void custom_hooks_resize(void) {
    if (custom_hooks_arr_size == 0) {
        custom_hooks_arr_size = 512;
        custom_hooks = (hook_t *) malloc(custom_hooks_arr_size * sizeof(hook_t));
    } else {
        hook_t *new_array;

        custom_hooks_arr_size *= 2;
        new_array = (hook_t *) malloc(custom_hooks_arr_size * sizeof(hook_t));
        memcpy(&new_array[0], &custom_hooks[0], custom_hooks_size * sizeof(hook_t));
        free(custom_hooks);
        custom_hooks = new_array;
    }
}

void add_custom_hook(char *name, void *addr) {
    int i;

    if (custom_hooks_size + 1 >= custom_hooks_arr_size) {
        custom_hooks_resize();
    }
    for (i = 0; i < custom_hooks_size; ++i) {
        if (strcmp(custom_hooks[i].name, name) == 0) {
            custom_hooks[i].name = name;
            custom_hooks[i].addr = addr;
            return;
        }
    }
    custom_hooks[custom_hooks_size].name = name;
    custom_hooks[custom_hooks_size++].addr = addr;
}

void *get_hooked_symbol(char *name) {
    int i;
    hook_t *hook;

    for (i = 0; i < custom_hooks_size; ++i) {
        if (strcmp(name, custom_hooks[i].name) == 0) {
            return custom_hooks[i].addr;
        }
    }
    for (hook = &hooks[0]; hook->name != NULL; ++hook) {
        if (strcmp(name, hook->name) == 0) {
            return hook->addr;
        }
    }
    for (hook = &pthread_hooks[0]; hook->name != NULL; ++hook) {
        if (strcmp(name, hook->name) == 0) {
            return hook->addr;
        }
    }
    for (hook = &math_hooks[0]; hook->name != NULL; ++hook) {
        if (strcmp(name, hook->name) == 0) {
            return hook->addr;
        }
    }
    return NULL;
}
