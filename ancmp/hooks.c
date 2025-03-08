#include "hooks.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <float.h>
#include <stdio.h>
#include <signal.h>
#include <wchar.h>
#include <strings.h>
#include <locale.h>
#include <wctype.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>
#include "android_pthread.h"
#include "android_math.h"
#include "android_io.h"
#include "android_tolower.h"
#include "android_stat.h"
#include "android_dirent.h"
#include "android_socket.h"
#include "android_posix_types.h"
#include "android_fcntl.h"
#include "android_ioctl.h"
#include "android_mmap.h"
#include <sys/stat.h>
#include <fcntl.h>
#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/select.h>
#include <netdb.h>
#endif

typedef struct {
    char *name;
    void *addr;
} hook_t;

void *__stack_chk_guard = NULL;

#ifdef _WIN32

int android_mkdir(const char *pathname, mode_t mode) {
    return mkdir(pathname);
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

long android_sysconf(int name) {
    if (name == 0x0027) {
        return 4096;
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
    if (GetComputerNameEx(ComputerNameDnsHostname, name, (DWORD *)&size)) {
        return 0;
    }
    return -1;
}
#else
#define android_mkdir mkdir
#define android_pipe pipe
#define android_sysconf sysconf
#define android_select select
#define android_gethostname gethostname
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

android_hostent_t *android_gethostbyname(const char *name) {
    struct addrinfo *info;
    
    if(getaddrinfo(name, NULL, NULL, &info) == 0) {
        int i = 0;
        struct addrinfo *original_info = info;
        for (i = 0; (i < 34) && (info != NULL); (++i) && (info = info->ai_next)) {
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

int android_usleep(unsigned long usec) {
#ifdef _WIN32
    Sleep(usec / 1000);
    return 0;
#else
    struct timespec ts;
    ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;
    return nanosleep(&ts, NULL);
#endif
}

void __cxa_finalize(void * d) {}
int __cxa_atexit(void (*func) (void *), void * arg, void * dso_handle) {
    //return atexit((void *)func);
    return 0;
}

void __stack_chk_fail() {
    puts("__stack_chk_fail");
}

void __cxa_pure_virtual() {}

int __isinff(float x) {
    return isinf(x);
}

int __isfinitef(float x) {
    return isfinite(x);
}
 
int __isfinite(double x) {
    return isfinite(x);
}

int *android_errno() {
    int *ret = &errno;
    return ret;
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
        .name = (char *)NULL,
        .addr = NULL
    }
};

static hook_t hooks[] = {
    {
        .name = "__cxa_finalize",
        .addr = __cxa_finalize
    },
    {
        .name = "__cxa_atexit",
        .addr = __cxa_atexit
    },
    {
        .name = "__cxa_pure_virtual",
        .addr = __cxa_pure_virtual
    },
    {
        .name = "__stack_chk_fail",
        .addr = __stack_chk_fail
    },
    {
        .name = "__stack_chk_guard",
        .addr = &__stack_chk_guard
    },
    {
        .name = "ldexp",
        .addr = ldexp
    },
    {
        .name = "__isnanf",
        .addr = __isnanf
    },
    {
        .name = "__isinff",
        .addr = __isinff
    },
    {
        .name = "__isfinitef",
        .addr = __isfinitef
    },
    {
        .name = "__isfinite",
        .addr = __isfinite
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
        .addr = div
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
        .addr = strcasecmp
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
        .addr = strerror
    },
    {
        .name = "strlen",
        .addr = strlen
    },
    {
        .name = "strncasecmp",
        .addr = strncasecmp
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
        .addr = inet_ntoa
    },
    {
        .name = "inet_addr",
        .addr = inet_addr
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
        .addr = fopen
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
        .addr = rename
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
        .addr = gettimeofday
    },
    {
        .name = "fstat",
        .addr = android_fstat
    },
    {
        .name = "closedir",
        .addr = closedir
    },
    {
        .name = "opendir",
        .addr = opendir
    },
    {
        .name = "readdir",
        .addr = android_readdir
    },
    {
        .name = "remove",
        .addr = remove
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
        .addr = getsockname
    },
    {
        .name = "shutdown",
        .addr = shutdown
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
    for (size_t i = 0; i < custom_hooks_size; ++i) {
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