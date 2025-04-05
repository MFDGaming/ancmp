#include "android_stat.h"
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

static void native_to_android(struct stat *from, android_stat_t *to) {
    to->st_dev = from->st_dev;
    to->st_nlink = from->st_nlink;
    to->st_uid = from->st_uid;
    to->st_gid = from->st_gid;
    to->st_rdev = from->st_rdev;
    to->st_size = from->st_size;
    to->st_ino = from->st_ino;
    to->__st_ino = from->st_ino;
#ifdef _WIN32
    to->st_mode = 0;
    if ((from->st_mode & _S_IFMT) == _S_IFDIR) {
        to->st_mode |= ANDROID_S_IFDIR;
    }
    if ((from->st_mode & _S_IFMT) == _S_IFCHR) {
        to->st_mode |= ANDROID_S_IFCHR;
    }
    if ((from->st_mode & _S_IFMT) == _S_IFREG) {
        to->st_mode |= ANDROID_S_IFREG;
    }
    if (from->st_mode & _S_IREAD) {
        to->st_mode |= ANDROID_S_IRUSR | ANDROID_S_IRGRP | ANDROID_S_IROTH;
    }
    if (from->st_mode & _S_IWRITE) {
        to->st_mode |= ANDROID_S_IWUSR | ANDROID_S_IWGRP | ANDROID_S_IWOTH;
    }
    if (from->st_mode & _S_IEXEC) {
        to->st_mode |= ANDROID_S_IXUSR | ANDROID_S_IXGRP | ANDROID_S_IXOTH;
    }
    to->_st_atime = from->st_atime;
    to->_st_mtime = from->st_mtime;
    to->_st_ctime = from->st_mtime;
    to->_st_atime_nsec = 0;
    to->_st_mtime_nsec = 0;
    to->_st_ctime_nsec = 0;
    to->st_blksize = 4096;
    to->st_blocks = (((from->st_size / to->st_blksize) + ((from->st_size % to->st_blksize) ? 1 : 0)) * to->st_blksize) / 512;
#else
    to->st_mode = s_to_android(from->st_mode);
    to->_st_atime = from->st_atim.tv_sec;
    to->_st_mtime = from->st_mtim.tv_sec;
    to->_st_ctime = from->st_mtim.tv_sec;
    to->_st_atime_nsec = from->st_atim.tv_nsec;
    to->_st_mtime_nsec = from->st_mtim.tv_nsec;
    to->_st_ctime_nsec = from->st_ctim.tv_nsec;
    to->st_blksize = from->st_blksize;
    to->st_blocks = from->st_blocks;
#endif
}
#ifndef _WIN32
int s_to_android(int mode) {
    int real_mode = 0;
#ifdef S_IFMT
    if (mode & S_IFMT) real_mode |= ANDROID_S_IFMT;
#endif
#ifdef S_IFSOCK
    if (mode & S_IFSOCK) real_mode |= ANDROID_S_IFSOCK;
#endif
#ifdef S_IFLNK
    if (mode & S_IFLNK) real_mode |= ANDROID_S_IFLNK;
#endif
#ifdef S_IFREG
    if (mode & S_IFREG) real_mode |= ANDROID_S_IFREG;
#endif
#ifdef S_IFBLK
    if (mode & S_IFBLK) real_mode |= ANDROID_S_IFBLK;
#endif
#ifdef S_IFDIR
    if (mode & S_IFDIR) real_mode |= ANDROID_S_IFDIR;
#endif
#ifdef S_IFCHR
    if (mode & S_IFCHR) real_mode |= ANDROID_S_IFCHR;
#endif
#ifdef S_IFIFO
    if (mode & S_IFIFO) real_mode |= ANDROID_S_IFIFO;
#endif
#ifdef S_ISUID
    if (mode & S_ISUID) real_mode |= ANDROID_S_ISUID;
#endif
#ifdef S_ISGID
    if (mode & S_ISGID) real_mode |= ANDROID_S_ISGID;
#endif
#ifdef S_ISVTX
    if (mode & S_ISVTX) real_mode |= ANDROID_S_ISVTX;
#endif
#ifdef S_IRUSR
    if (mode & S_IRUSR) real_mode |= ANDROID_S_IRUSR;
#endif
#ifdef S_IWUSR
    if (mode & S_IWUSR) real_mode |= ANDROID_S_IWUSR;
#endif
#ifdef S_IXUSR
    if (mode & S_IXUSR) real_mode |= ANDROID_S_IXUSR;
#endif
#ifdef S_IRGRP
    if (mode & S_IRGRP) real_mode |= ANDROID_S_IRGRP;
#endif
#ifdef S_IWGRP
    if (mode & S_IWGRP) real_mode |= ANDROID_S_IWGRP;
#endif
#ifdef S_IXGRP
    if (mode & S_IXGRP) real_mode |= ANDROID_S_IXGRP;
#endif
#ifdef S_IROTH
    if (mode & S_IROTH) real_mode |= ANDROID_S_IROTH;
#endif
#ifdef S_IWOTH
    if (mode & S_IWOTH) real_mode |= ANDROID_S_IWOTH;
#endif
#ifdef S_IXOTH
    if (mode & S_IXOTH) real_mode |= ANDROID_S_IXOTH;
#endif
    return real_mode;
}

int s_to_native(int mode) {
    int real_mode = 0;
#ifdef S_IFMT
    if (mode & ANDROID_S_IFMT) real_mode |= S_IFMT;
#endif
#ifdef S_IFSOCK
    if (mode & ANDROID_S_IFSOCK) real_mode |= S_IFSOCK;
#endif
#ifdef S_IFLNK
    if (mode & ANDROID_S_IFLNK) real_mode |= S_IFLNK;
#endif
#ifdef S_IFREG
    if (mode & ANDROID_S_IFREG) real_mode |= S_IFREG;
#endif
#ifdef S_IFBLK
    if (mode & ANDROID_S_IFBLK) real_mode |= S_IFBLK;
#endif
#ifdef S_IFDIR
    if (mode & ANDROID_S_IFDIR) real_mode |= S_IFDIR;
#endif
#ifdef S_IFCHR
    if (mode & ANDROID_S_IFCHR) real_mode |= S_IFCHR;
#endif
#ifdef S_IFIFO
    if (mode & ANDROID_S_IFIFO) real_mode |= S_IFIFO;
#endif
#ifdef S_ISUID
    if (mode & ANDROID_S_ISUID) real_mode |= S_ISUID;
#endif
#ifdef S_ISGID
    if (mode & ANDROID_S_ISGID) real_mode |= S_ISGID;
#endif
#ifdef S_ISVTX
    if (mode & ANDROID_S_ISVTX) real_mode |= S_ISVTX;
#endif
#ifdef S_IRUSR
    if (mode & ANDROID_S_IRUSR) real_mode |= S_IRUSR;
#endif
#ifdef S_IWUSR
    if (mode & ANDROID_S_IWUSR) real_mode |= S_IWUSR;
#endif
#ifdef S_IXUSR
    if (mode & ANDROID_S_IXUSR) real_mode |= S_IXUSR;
#endif
#ifdef S_IRGRP
    if (mode & ANDROID_S_IRGRP) real_mode |= S_IRGRP;
#endif
#ifdef S_IWGRP
    if (mode & ANDROID_S_IWGRP) real_mode |= S_IWGRP;
#endif
#ifdef S_IXGRP
    if (mode & ANDROID_S_IXGRP) real_mode |= S_IXGRP;
#endif
#ifdef S_IROTH
    if (mode & ANDROID_S_IROTH) real_mode |= S_IROTH;
#endif
#ifdef S_IWOTH
    if (mode & ANDROID_S_IWOTH) real_mode |= S_IWOTH;
#endif
#ifdef S_IXOTH
    if (mode & ANDROID_S_IXOTH) real_mode |= S_IXOTH;
#endif
    return real_mode;
}
#endif

int android_fstat(int fd, android_stat_t *statbuf) {
    struct stat tmp;
    int ret = fstat(fd, &tmp);
    native_to_android(&tmp, statbuf);
    if (ret == 0) {
        printf("\x1b[32mSuccessfully ran fstat() for %d\x1b[0m\n", fd);
    } else {
        printf("\x1b[31mFailed to run fstat() for %d due to %d\x1b[0m\n", fd, errno);
    }
    return ret;
}

int android_stat(const char *pathname, android_stat_t *statbuf) {
    struct stat tmp;
    int ret = stat(pathname, &tmp);
    native_to_android(&tmp, statbuf);
    if (ret == 0) {
        printf("\x1b[32mSuccessfully ran stat() for %s\x1b[0m\n", pathname);
    } else {
        printf("\x1b[31mFailed to run stat() for %s due to %d\x1b[0m\n", pathname, errno);
    }
    return ret;
}
