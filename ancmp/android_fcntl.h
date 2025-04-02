#ifndef ANCMP_ANDROID_FCNTL_H
#define ANCMP_ANDROID_FCNTL_H

#define ANDROID_F_DUPFD 0  
#define ANDROID_F_GETFD 1  
#define ANDROID_F_SETFD 2  
#define ANDROID_F_GETFL 3  
#define ANDROID_F_SETFL 4
#define ANDROID_F_GETLK 5
#define ANDROID_F_SETLK 6
#define ANDROID_F_SETLKW 7

#define ANDROID_O_NONBLOCK 00004000
#define ANDROID_O_APPEND 00002000
#define ANDROID_O_RDONLY 00000000
#define ANDROID_O_WRONLY 00000001
#define ANDROID_O_RDWR 00000002
#define ANDROID_O_ACCMODE 00000003
#define ANDROID_O_CREAT 00000100  
#define ANDROID_O_EXCL 00000200  
#define ANDROID_O_TRUNC 00001000

#define ANDROID_F_RDLCK 0
#define ANDROID_F_WRLCK 1
#define ANDROID_F_UNLCK 2

typedef struct {
    short l_type;
    short l_whence;
    long l_start;
    long l_len;
    int l_pid;
} android_flock_t;

#ifdef _WIN32
int is_socket(int fd);

int android_fcntl(int fd, int op, ...);

int android_open(const char *pathname, int flags, ...);
#else
#include <fcntl.h>
#define android_fcntl fcntl
#define android_open open
#endif

#endif
