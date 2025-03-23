#pragma once

#include <stdint.h>

typedef struct {
    uint64_t          d_ino;
    int64_t           d_off;
    unsigned short    d_reclen;
    unsigned char     d_type;
    char              d_name[256];
} android_dirent_t;

#ifdef _WIN32

#include <windows.h>

typedef struct android_DIR {
    HANDLE hFind;
    WIN32_FIND_DATA findFileData;
} android_DIR;

android_DIR *android_opendir(const char *name);

int android_closedir(android_DIR *dirp);

android_dirent_t *android_readdir(android_DIR *dirp);

#else

#include <dirent.h>

#define android_DIR DIR
#define android_opendir opendir
#define android_closedir closedir
android_dirent_t *android_readdir(DIR *dirp);

#endif
