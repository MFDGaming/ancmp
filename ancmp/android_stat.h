#ifndef ANCMP_ANDROID_STAT_H
#define ANCMP_ANDROID_STAT_H

#include "ancmp_stdint.h"

#define ANDROID_S_IFMT 00170000
#define ANDROID_S_IFSOCK 0140000
#define ANDROID_S_IFLNK 0120000
#define ANDROID_S_IFREG 0100000
#define ANDROID_S_IFBLK 0060000
#define ANDROID_S_IFDIR 0040000
#define ANDROID_S_IFCHR 0020000
#define ANDROID_S_IFIFO 0010000
#define ANDROID_S_ISUID 0004000
#define ANDROID_S_ISGID 0002000
#define ANDROID_S_ISVTX 0001000
#define ANDROID_S_IRWXU 00700
#define ANDROID_S_IRUSR 00400
#define ANDROID_S_IWUSR 00200
#define ANDROID_S_IXUSR 00100
#define ANDROID_S_IRWXG 00070
#define ANDROID_S_IRGRP 00040
#define ANDROID_S_IWGRP 00020
#define ANDROID_S_IXGRP 00010
#define ANDROID_S_IRWXO 00007
#define ANDROID_S_IROTH 00004
#define ANDROID_S_IWOTH 00002
#define ANDROID_S_IXOTH 00001

#define ANDROID_S_ISLNK(m) (((m) & ANDROID_S_IFMT) == ANDROID_S_IFLNK)
#define ANDROID_S_ISREG(m) (((m) & ANDROID_S_IFMT) == ANDROID_S_IFREG)
#define ANDROID_S_ISDIR(m) (((m) & ANDROID_S_IFMT) == ANDROID_S_IFDIR)
#define ANDROID_S_ISCHR(m) (((m) & ANDROID_S_IFMT) == ANDROID_S_IFCHR)
#define ANDROID_S_ISBLK(m) (((m) & ANDROID_S_IFMT) == ANDROID_S_IFBLK)
#define ANDROID_S_ISFIFO(m) (((m) & ANDROID_S_IFMT) == ANDROID_S_IFIFO)
#define ANDROID_S_ISSOCK(m) (((m) & ANDROID_S_IFMT) == ANDROID_S_IFSOCK)

typedef struct {
    uint64_t st_dev;
    unsigned char __pad0[4];

    unsigned long __st_ino;
    unsigned int st_mode;
    unsigned int st_nlink;

    unsigned int st_uid;
    unsigned int st_gid;

    uint64_t st_rdev;
    unsigned char __pad3[4];

    int64_t st_size;
    unsigned long st_blksize;
    uint64_t st_blocks;

    unsigned long _st_atime;
    unsigned long _st_atime_nsec;

    unsigned long _st_mtime;
    unsigned long _st_mtime_nsec;

    unsigned long _st_ctime;
    unsigned long _st_ctime_nsec;

    uint64_t st_ino;
} android_stat_t;

int s_to_native(int mode);

int s_to_android(int mode);

int android_fstat(int fd, android_stat_t *statbuf);

int android_stat(const char *pathname, android_stat_t *statbuf);

#endif
