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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/timeb.h>
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
#include "posix_funcs.h"
#include "android_cxa.h"
#include "android_strcasecmp.h"
#include "android_errno.h"

typedef struct {
    char *name;
    void *addr;
} hook_t;

void *android_stack_chk_guard = NULL;

#ifdef _WIN32

int android_mkdir(const char *pathname, int mode) {
    puts("android_mkdir");
    int ret = mkdir(pathname);
    if (ret != 0) {
        printf("\x1b[31mFailed to mkdir %s due to %d\x1b[0m\n", pathname, errno);
    }
    return ret;
}

int android_pipe(int fds[2]) {
    puts("android_pipe");
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

int android_getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    puts("android_getsockname");
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
    puts("android_select");
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
    puts("android_gethostname");
    DWORD len = size - 1;
    if (GetComputerNameEx(ComputerNameDnsHostname, name, &len)) {
        name[size - 1] = '\0';
        return 0;
    }
    memset(name, 0, size);
    return -1;
}

char *android_inet_ntoa(uint32_t addr) {
    //puts("android_inet_ntoa");
    struct in_addr real_addr;
    memcpy(&real_addr, &addr, sizeof(uint32_t));
    return inet_ntoa(real_addr);
}

uint32_t android_inet_addr(char *addr) {
    puts("android_inet_addr");
    return inet_addr(addr);
}

int android_gettimeofday(struct timeval *tp, struct timezone *tzp) {
	FILETIME	file_time;
	SYSTEMTIME	system_time;
	ULARGE_INTEGER ularge;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	ularge.LowPart = file_time.dwLowDateTime;
	ularge.HighPart = file_time.dwHighDateTime;

	tp->tv_sec = (long) ((ularge.QuadPart - 116444736000000000ULL) / 10000000L);
	tp->tv_usec = (long) (system_time.wMilliseconds * 1000);

	return 0;
}

void android_div(div_t *ret, int numerator, int denominator) {
    puts("android_div");
    ret->quot = numerator/denominator;
    ret->rem = numerator%denominator;
}


static uint64_t seed48[3];

static const uint64_t multiplier = 0x5DEECE66D;
static const uint64_t addend = 0xB;
static const uint64_t mask = (1ULL << 48) - 1;

void android_srand48(long seedval) {
    puts("android_srand48");
    seed48[0] = (seedval ^ multiplier) & mask;
    seed48[1] = 0x330E;
    seed48[2] = 0x0;
}

long android_lrand48() {
    puts("android_lrand48");
    seed48[0] = (multiplier * seed48[0] + addend) & mask;
    return (long)(seed48[0] >> 16) & 0x7FFFFFFF;
}

FLOAT_ABI_FIX double android_drand48() {
    puts("android_drand48");
    return (double)android_lrand48() / (1ULL << 31);
}

int android_nanosleep(const struct timespec *ts, struct timespec *rem){
    puts("android_nanosleep");
    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    if(!timer)
        return -1;

    time_t sec = ts->tv_sec + ts->tv_nsec / 1000000000L;
    long nsec = ts->tv_nsec % 1000000000L;

    LARGE_INTEGER delay;
    delay.QuadPart = -(sec * 1000000000L + nsec) / 100;
    BOOL ok = SetWaitableTimer(timer, &delay, 0, NULL, NULL, FALSE) &&
              WaitForSingleObject(timer, INFINITE) == WAIT_OBJECT_0;

    CloseHandle(timer);

    if(!ok)
      return -1;

    return 0;
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
    puts("android_writev");
    int cnt = 0;
    for (int i = 0; i < iovcnt; ++i) {
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

int android_sched_yield() {
    puts("android_sched_yield");
    HANDLE ev = CreateEvent(NULL, FALSE, FALSE, NULL);
    WaitForSingleObject(ev, 0);
    CloseHandle(ev);
    return -1;
}

struct tm *android_localtime_r(const time_t *timep, struct tm *result) {
    puts("android_localtime_r");
    struct tm *res = localtime(timep);
    if (res) {
        memcpy(result, res, sizeof(struct tm));
        return result;
    }
    return NULL;
}

int android_fsync(int fd) {
    puts("android_fsync");
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
    puts("android_fdatasync");
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

int android_geteuid() {
    puts("android_geteuid");
    return 0;
}

int android_getpid() {
    return GetCurrentProcessId();
}

int android_remove(const char *pathname) {
    puts("android_remove");
    int ret = remove(pathname);
    if (ret != 0) {
        printf("\x1b[31mFailed to remove %s due to %d\x1b[0m\n", pathname, errno);
    }
    return ret;
}

int android_rename(const char *oldpath, const char *newpath) {
    puts("android_rename");
    BOOL ret = MoveFileEx(oldpath, newpath, MOVEFILE_REPLACE_EXISTING);
    if (!ret ) {
        printf("\x1b[31mFailed to rename %s to %s due to %d\x1b[0m\n", oldpath, newpath, GetLastError());
    }
    return ret ? 0 : -1;
}

int android_unlink(const char *pathname) {
    puts("android_unlink");
    int ret = unlink(pathname);
    if (ret != 0) {
        printf("\x1b[31mFailed to unlink %s due to %d\x1b[0m\n", pathname, errno);
    }
    return ret;
}

int android_shutdown(int sockfd, int how) {
    return shutdown(sockfd, how);
}

#else
#define android_mkdir mkdir
#define android_pipe pipe
#define android_select select
#define android_gethostname gethostname
#define android_getsockname getsockname
#define android_gettimeofday gettimeofday
#define android_div div
#define android_inet_ntoa inet_ntoa
#define android_inet_addr inet_addr
#define android_lrand48 lrand48
#define android_srand48 srand48
#define android_nanosleep nanosleep
#define android_syscall syscall
#define android_writev writev
#define android_poll poll
#define android_sigaction sigaction
#define android_sched_yield sched_yield
#define android_localtime_r localtime_r
#define android_fsync fsync
#define android_fdatasync fdatasync
#define android_geteuid geteuid
#define android_getpid getpid
#define android_remove remove
#define android_rename rename
#define android_unlink unlink
#define android_shutdown shutdown
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
    .h_name = h_name,
    .h_addr_list = h_addr_list,
    .h_aliases = h_aliases,
    .h_length = 4,
    .h_addrtype = ANDROID_AF_INET
};

#ifdef HAS_GETADDRINFO
android_hostent_t *android_gethostbyname(const char *name) {
    struct addrinfo *info;
    
    if(getaddrinfo(name, NULL, NULL, &info) == 0) {
        int i = 0;
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

int android_usleep(unsigned long usec) {
    struct timespec ts;
    ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;
    return android_nanosleep(&ts, NULL);
}

FLOAT_ABI_FIX double android_strtod(const char *nptr, char **endptr) {
    return strtod(nptr, endptr);
}

void android_stack_chk_fail() {
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

void android_assert2(const char* __file, int __line, const char* __function, const char* __msg) {
    printf("Assertion failed: %s\nFile: %s\nLine: %d\nFunction: %s\n", __msg, __file, __line, __function);
    abort();
}

static hook_t pthread_hooks[] = {
    {
        .name = "pthread_attr_init",
        .addr = android_pthread_attr_init
    },
    {
        .name = "pthread_attr_destroy",
        .addr = android_pthread_attr_destroy
    },
    {
        .name = "pthread_attr_setdetachstate",
        .addr = android_pthread_attr_setdetachstate
    },
    {
        .name = "pthread_attr_setschedparam",
        .addr = android_pthread_attr_setschedparam
    },
    {
        .name = "pthread_attr_setstacksize",
        .addr = android_pthread_attr_setstacksize
    },
    {
        .name = "pthread_cond_broadcast",
        .addr = android_pthread_cond_broadcast
    },
    {
        .name = "pthread_cond_destroy",
        .addr = android_pthread_cond_destroy
    },
    {
        .name = "pthread_cond_init",
        .addr = android_pthread_cond_init
    },
    {
        .name = "pthread_mutex_trylock",
        .addr = android_pthread_mutex_trylock
    },
    {
        .name = "pthread_cond_timedwait",
        .addr = android_pthread_cond_timedwait
    },
    {
        .name = "pthread_cond_wait",
        .addr = android_pthread_cond_wait
    },
    {
        .name = "pthread_create",
        .addr = android_pthread_create
    },
    {
        .name = "pthread_getspecific",
        .addr = android_pthread_getspecific
    },
    {
        .name = "pthread_join",
        .addr = android_pthread_join
    },
    {
        .name = "pthread_key_create",
        .addr = android_pthread_key_create
    },
    {
        .name = "pthread_mutexattr_destroy",
        .addr = android_pthread_mutexattr_destroy
    },
    {
        .name = "pthread_mutexattr_init",
        .addr = android_pthread_mutexattr_init
    },
    {
        .name = "pthread_mutex_destroy",
        .addr = android_pthread_mutex_destroy
    },
    {
        .name = "pthread_mutex_init",
        .addr = android_pthread_mutex_init
    },
    {
        .name = "pthread_mutex_lock",
        .addr = android_pthread_mutex_lock
    },
    {
        .name = "pthread_mutex_unlock",
        .addr = android_pthread_mutex_unlock
    },
    {
        .name = "pthread_self",
        .addr = android_pthread_self
    },
    {
        .name = "pthread_setspecific",
        .addr = android_pthread_setspecific
    },
    {
        .name = "pthread_key_delete",
        .addr = android_pthread_key_delete
    },
    {
        .name = "pthread_once",
        .addr = android_pthread_once
    },
    {
        .name = "pthread_equal",
        .addr = android_pthread_equal
    },
    {
        .name = "pthread_cond_signal",
        .addr = android_pthread_cond_signal
    },
    {
        .name = "pthread_detach",
        .addr = android_pthread_detach
    },
    {
        .name = "pthread_setname_np",
        .addr = android_pthread_setname_np
    },
    {
        .name = (char *)NULL,
        .addr = NULL
    }
};

static hook_t math_hooks[] = {
    {
        .name = "atan2f",
        .addr = android_atan2f
    },
    {
        .name = "ceilf",
        .addr = android_ceilf
    },
    {
        .name = "cosf",
        .addr = android_cosf
    },
    {
        .name = "floorf",
        .addr = android_floorf
    },
    {
        .name = "fmodf",
        .addr = android_fmodf
    },
    {
        .name = "logf",
        .addr = android_logf
    },
    {
        .name = "powf",
        .addr = android_powf
    },
    {
        .name = "sinf",
        .addr = android_sinf
    },
    {
        .name = "sqrtf",
        .addr = android_sqrtf
    },
    {
        .name = "atanf",
        .addr = android_atanf
    },
    {
        .name = "fmod",
        .addr = android_fmod
    },
    {
        .name = "floor",
        .addr = android_floor
    },
    {
        .name = "ceil",
        .addr = android_ceil
    },
    {
        .name = "sin",
        .addr = android_sin
    },
    {
        .name = "cos",
        .addr = android_cos
    },
    {
        .name = "sqrt",
        .addr = android_sqrt
    },
    {
        .name = "pow",
        .addr = android_pow
    },
    {
        .name = "atan2",
        .addr = android_atan2
    },
    {
        .name = "atan",
        .addr = android_atan
    },
    {
        .name = "modff",
        .addr = android_modff
    },
    {
        .name = "modf",
        .addr = android_modf
    },
    {
        .name = "ldexp",
        .addr = android_ldexp
    },
    {
        .name = "ldexpf",
        .addr = android_ldexpf
    },
    {
        .name = "tanf",
        .addr = android_tanf
    },
    {
        .name = (char *)NULL,
        .addr = NULL
    }
};

static hook_t hooks[] = {
    {
        .name = "__cxa_finalize",
        .addr = android_cxa_finalize
    },
    {
        .name = "__cxa_atexit",
        .addr = android_cxa_atexit
    },
    {
        .name = "__cxa_pure_virtual",
        .addr = android_cxa_pure_virtual
    },
    {
        .name = "__stack_chk_fail",
        .addr = android_stack_chk_fail
    },
    {
        .name = "__stack_chk_guard",
        .addr = &android_stack_chk_guard
    },
    {
        .name = "__strlen_chk",
        .addr = android_strlen_chk
    },
    {
        .name = "__isnanf",
        .addr = android_isnanf
    },
    {
        .name = "__isinff",
        .addr = android_isinff
    },
    {
        .name = "__isfinitef",
        .addr = android_isfinitef
    },
    {
        .name = "__isfinite",
        .addr = android_isfinite
    },
    {
        .name = "bsd_signal",
        .addr = signal
    },
    {
        .name = "malloc",
        .addr = malloc
    },
    {
        .name = "realloc",
        .addr = realloc
    },
    {
        .name = "free",
        .addr = free
    },
    {
        .name = "exit",
        .addr = exit
    },
    {
        .name = "abort",
        .addr = abort
    },
    {
        .name = "div",
        .addr = android_div
    },
    {
        .name = "memchr",
        .addr = memchr
    },
    {
        .name = "memcmp",
        .addr = memcmp
    },
    {
        .name = "memmove",
        .addr = memmove
    },
    {
        .name = "memset",
        .addr = memset
    },
    {
        .name = "wmemcpy",
        .addr = wmemcpy
    },
    {
        .name = "memcpy",
        .addr = memcpy
    },
    {
        .name = "wmemmove",
        .addr = wmemmove
    },
    {
        .name = "wmemset",
        .addr = wmemset
    },
    {
        .name = "isalpha",
        .addr = isalpha
    },
    {
        .name = "iscntrl",
        .addr = iscntrl
    },
    {
        .name = "islower",
        .addr = islower
    },
    {
        .name = "isprint",
        .addr = isprint
    },
    {
        .name = "ispunct",
        .addr = ispunct
    },
    {
        .name = "isspace",
        .addr = isspace
    },
    {
        .name = "isupper",
        .addr = isupper
    },
    {
        .name = "isxdigit",
        .addr = isxdigit
    },
    {
        .name = "iswalpha",
        .addr = iswalpha
    },
    {
        .name = "iswcntrl",
        .addr = iswcntrl
    },
    {
        .name = "iswdigit",
        .addr = iswdigit
    },
    {
        .name = "iswlower",
        .addr = iswlower
    },
    {
        .name = "iswprint",
        .addr = iswprint
    },
    {
        .name = "iswpunct",
        .addr = iswpunct
    },
    {
        .name = "iswspace",
        .addr = iswspace
    },
    
    {
        .name = "iswupper",
        .addr = iswupper
    },
    {
        .name = "iswxdigit",
        .addr = iswxdigit
    },
    {
        .name = "strcasecmp",
        .addr = android_strcasecmp
    },
    {
        .name = "strcat",
        .addr = strcat
    },
    {
        .name = "strchr",
        .addr = strchr
    },
    {
        .name = "strcmp",
        .addr = strcmp
    },
    {
        .name = "strcpy",
        .addr = strcpy
    },
    {
        .name = "strerror",
        .addr = android_strerror
    },
    {
        .name = "strlen",
        .addr = strlen
    },
    {
        .name = "strncasecmp",
        .addr = android_strncasecmp
    },
    {
        .name = "strncpy",
        .addr = strncpy
    },
    {
        .name = "strstr",
        .addr = strstr
    },
    {
        .name = "strtok",
        .addr = strtok
    },
    {
        .name = "strtoull",
        .addr = strtoull
    },
    {
        .name = "wcscmp",
        .addr = wcscmp
    },
    {
        .name = "wcslen",
        .addr = wcslen
    },
    {
        .name = "wcsncpy",
        .addr = wcsncpy
    },
    {
        .name = "setlocale",
        .addr = setlocale
    },
    {
        .name = "__errno",
        .addr = android_errno
    },
    {
        .name = "towupper",
        .addr = towupper
    },
    {
        .name = "towlower",
        .addr = towlower
    },
    {
        .name = "toupper",
        .addr = toupper
    },
    {
        .name = "tolower",
        .addr = tolower
    },
    {
        .name = "putchar",
        .addr = putchar
    },
    {
        .name = "printf",
        .addr = printf
    },
    {
        .name = "snprintf",
        .addr = snprintf
    },
    {
        .name = "sprintf",
        .addr = sprintf
    },
    {
        .name = "sscanf",
        .addr = sscanf
    },
    {
        .name = "puts",
        .addr = puts
    },
    {
        .name = "fflush",
        .addr = android_fflush
    },
    {
        .name = "fseek",
        .addr = android_fseek
    },
    {
        .name = "fgetpos",
        .addr = android_fgetpos
    },
    {
        .name = "fputs",
        .addr = android_fputs
    },
    {
        .name = "inet_ntoa",
        .addr = android_inet_ntoa
    },
    {
        .name = "inet_addr",
        .addr = android_inet_addr
    },
    {
        .name = "fsetpos",
        .addr = android_fsetpos
    },
    {
        .name = "ftell",
        .addr = android_ftell
    },
    {
        .name = "fwrite",
        .addr = android_fwrite
    },
    {
        .name = "fread",
        .addr = android_fread
    },
    {
        .name = "ungetc",
        .addr = android_ungetc
    },
    {
        .name = "fopen",
        .addr = android_fopen
    },
    {
        .name = "fclose",
        .addr = android_fclose
    },
    {
        .name = "putc",
        .addr = android_putc
    },
    {
        .name = "setvbuf",
        .addr = android_setvbuf
    },
    {
        .name = "rename",
        .addr = android_rename
    },
    {
        .name = "__sF",
        .addr = &android_sf
    },
    {
        .name = "_tolower_tab_",
        .addr = &android_tolower_tab
    },
    {
        .name = "_ctype_",
        .addr = &android_ctype
    },
    {
        .name = "vsnprintf",
        .addr = vsnprintf
    },
    {
        .name = "time",
        .addr = time
    },
    {
        .name = "mktime",
        .addr = mktime
    },
    {
        .name = "gettimeofday",
        .addr = android_gettimeofday
    },
    {
        .name = "fstat",
        .addr = android_fstat
    },
    {
        .name = "closedir",
        .addr = android_closedir
    },
    {
        .name = "opendir",
        .addr = android_opendir
    },
    {
        .name = "readdir",
        .addr = android_readdir
    },
    {
        .name = "remove",
        .addr = android_remove
    },
    {
        .name = "mkdir",
        .addr = android_mkdir
    },
    {
        .name = "access",
        .addr = access
    },
    {
        .name = "pipe",
        .addr = android_pipe
    },
    {
        .name = "sysconf",
        .addr = android_sysconf
    },
    {
        .name = "usleep",
        .addr = android_usleep
    },
    {
        .name = "gethostname",
        .addr = android_gethostname
    },
    {
        .name = "listen",
        .addr = android_listen
    },
    {
        .name = "send",
        .addr = android_send
    },
    {
        .name = "sendto",
        .addr = android_sendto
    },
    {
        .name = "recv",
        .addr = android_recv
    },
    {
        .name = "recvfrom",
        .addr = android_recvfrom
    },
    {
        .name = "getsockopt",
        .addr = android_getsockopt
    },
    {
        .name = "setsockopt",
        .addr = android_setsockopt
    },
    {
        .name = "socket",
        .addr = android_socket
    },
    {
        .name = "accept",
        .addr = android_accept
    },
    {
        .name = "bind",
        .addr = android_bind
    },
    {
        .name = "connect",
        .addr = android_connect
    },
    {
        .name = "gethostbyname",
        .addr = android_gethostbyname
    },
    {
        .name = "getsockname",
        .addr = android_getsockname
    },
    {
        .name = "shutdown",
        .addr = android_shutdown
    },
    {
        .name = "select",
        .addr = android_select
    },
    {
        .name = "fcntl",
        .addr = android_fcntl
    },
    {
        .name = "ioctl",
        .addr = android_ioctl
    },
    {
        .name = "mmap",
        .addr = android_mmap
    },
    {
        .name = "munmap",
        .addr = android_munmap
    },
    {
        .name = "lseek",
        .addr = lseek
    },
    {
        .name = "close",
        .addr = android_close
    },
    {
        .name = "read",
        .addr = android_read
    },
    {
        .name = "write",
        .addr = android_write
    },
    {
        .name = "open",
        .addr = android_open
    },
    {
        .name = "atoi",
        .addr = atoi
    },
    {
        .name = "getc",
        .addr = android_getc
    },
    {
        .name = "__assert2",
        .addr = android_assert2
    },
    {
        .name = "fprintf",
        .addr = android_fprintf
    },
    {
        .name = "fscanf",
        .addr = android_fscanf
    },
    {
        .name = "fgets",
        .addr = android_fgets
    },
    {
        .name = "calloc",
        .addr = calloc
    },
    {
        .name = "strpbrk",
        .addr = strpbrk
    },
    {
        .name = "lrand48",
        .addr = android_lrand48
    },
    {
        .name = "srand48",
        .addr = android_srand48
    },
    {
        .name = "ftime",
        .addr = ftime
    },
    {
        .name = "gmtime",
        .addr = gmtime
    },
    {
        .name = "getpid",
        .addr = android_getpid
    },
    {
        .name = "nanosleep",
        .addr = android_nanosleep
    },
    {
        .name = "syscall",
        .addr = android_syscall
    },
    {
        .name = "strtod",
        .addr = android_strtod
    },
    {
        .name = "wmemchr",
        .addr = wmemchr
    },
    {
        .name = "vsprintf",
        .addr = vsprintf
    },
    {
        .name = "wmemcmp",
        .addr = wmemcmp
    },
    {
        .name = "fputc",
        .addr = android_fputc
    },
    {
        .name = "wctype",
        .addr = wctype
    },
    {
        .name = "iswctype",
        .addr = iswctype
    },
    {
        .name = "wctob",
        .addr = wctob
    },
    {
        .name = "btowc",
        .addr = btowc
    },
    {
        .name = "wcrtomb",
        .addr = wcrtomb
    },
    {
        .name = "mbrtowc",
        .addr = mbrtowc
    },
    {
        .name = "strcoll",
        .addr = strcoll
    },
    {
        .name = "strxfrm",
        .addr = strxfrm
    },
    {
        .name = "wcscoll",
        .addr = wcscoll
    },
    {
        .name = "wcsxfrm",
        .addr = wcsxfrm
    },
    {
        .name = "strftime",
        .addr = strftime
    },
    {
        .name = "wcsftime",
        .addr = wcsftime
    },
    {
        .name = "putwc",
        .addr = android_putwc
    },
    {
        .name = "ungetwc",
        .addr = android_ungetwc
    },
    {
        .name = "getwc",
        .addr = android_getwc
    },
    {
        .name = "fdopen",
        .addr = android_fdopen
    },
    {
        .name = "writev",
        .addr = android_writev
    },
    {
        .name = "poll",
        .addr = android_poll
    },
    {
        .name = "strtol",
        .addr = strtol
    },
    {
        .name = "ferror",
        .addr = android_ferror
    },
    {
        .name = "feof",
        .addr = android_feof
    },
    {
        .name = "raise",
        .addr = raise
    },
    {
        .name = "rmdir",
        .addr = rmdir
    },
    {
        .name = "stat",
        .addr = android_stat
    },
    {
        .name = "sigaction",
        .addr = android_sigaction
    },
    {
        .name = "perror",
        .addr = perror
    },
    {
        .name = "sched_yield",
        .addr = android_sched_yield
    },
    {
        .name = "localtime_r",
        .addr = android_localtime_r
    },
    {
        .name = "pread",
        .addr = pread
    },
    {
        .name = "strrchr",
        .addr = strrchr
    },
    {
        .name = "fsync",
        .addr = android_fsync
    },
    {
        .name = "fdatasync",
        .addr = android_fdatasync
    },
    {
        .name = "unlink",
        .addr = android_unlink
    },
    {
        .name = "getenv",
        .addr = getenv
    },
    {
        .name = "geteuid",
        .addr = android_geteuid
    },
    {
        .name = "clock_gettime",
        .addr = clock_gettime
    },
    {
        .name = "dlopen",
        .addr = android_dlopen
    },
    {
        .name = "dlclose",
        .addr = android_dlclose
    },
    {
        .name = "dlsym",
        .addr = android_dlsym
    },
    {
        .name = "dladdr",
        .addr = android_dladdr
    },
    {
        .name = "dlerror",
        .addr = android_dlerror
    },
#ifdef ANDROID_ARM_LINKER
    {
        .name = "dl_iterate_phdr",
        .addr = android_dl_unwind_find_exidx
    },
#else
    {
        .name = "dl_iterate_phdr",
        .addr = android_dl_iterate_phdr
    },
#endif
    {
        .name = (char *)NULL,
        .addr = NULL
    }
};

static hook_t *custom_hooks = NULL;
static int custom_hooks_size = 0;
static int custom_hooks_arr_size = 0;

static void custom_hooks_resize() {
    if (custom_hooks_arr_size == 0) {
        custom_hooks_arr_size = 512;
        custom_hooks = (hook_t *) malloc(custom_hooks_arr_size * sizeof(hook_t));
    } else {
        custom_hooks_arr_size *= 2;
        hook_t *new_array = (hook_t *) malloc(custom_hooks_arr_size * sizeof(hook_t));
        memcpy(&new_array[0], &custom_hooks[0], custom_hooks_size * sizeof(hook_t));
        free(custom_hooks);
        custom_hooks = new_array;
    }
}

void add_custom_hook(char *name, void *addr) {
    if (custom_hooks_size + 1 >= custom_hooks_arr_size) {
        custom_hooks_resize();
    }
    for (int i = 0; i < custom_hooks_size; ++i) {
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
    for (int i = 0; i < custom_hooks_size; ++i) {
        if (strcmp(name, custom_hooks[i].name) == 0) {
            return custom_hooks[i].addr;
        }
    }
    for (hook_t *hook = &hooks[0]; hook->name != NULL; ++hook) {
        if (strcmp(name, hook->name) == 0) {
            return hook->addr;
        }
    }
    for (hook_t *hook = &pthread_hooks[0]; hook->name != NULL; ++hook) {
        if (strcmp(name, hook->name) == 0) {
            return hook->addr;
        }
    }
    for (hook_t *hook = &math_hooks[0]; hook->name != NULL; ++hook) {
        if (strcmp(name, hook->name) == 0) {
            return hook->addr;
        }
    }
    return NULL;
}