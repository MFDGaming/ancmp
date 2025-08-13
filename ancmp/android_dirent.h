#ifndef ANCMP_ANDROID_DIRENT_H
#define ANCMP_ANDROID_DIRENT_H

#include "ancmp_stdint.h"

#define ANDROID_DT_UNKNOWN     0
#define ANDROID_DT_FIFO        1
#define ANDROID_DT_CHR         2
#define ANDROID_DT_DIR         4
#define ANDROID_DT_BLK         6
#define ANDROID_DT_REG         8
#define ANDROID_DT_LNK         10
#define ANDROID_DT_SOCK        12
#define ANDROID_DT_WHT         14

typedef struct {
    struct {
        uint32_t l;
        uint32_t h;
    } d_ino;
    struct {
        uint32_t l;
        uint32_t h;
    } d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[256];
} android_dirent_t;

#ifdef _WIN32

#include <windows.h>

typedef struct android_DIR {
    HANDLE hFind;
    WIN32_FIND_DATAA findFileData;
    volatile long first;
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

#endif
