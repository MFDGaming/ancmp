#pragma once

#include <stdint.h>
#include <dirent.h>

typedef struct {
    uint64_t          d_ino;
    int64_t           d_off;
    unsigned short    d_reclen;
    unsigned char     d_type;
    char              d_name[256];
} android_dirent_t;

android_dirent_t *android_readdir(DIR *dirp);