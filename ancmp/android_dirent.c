#include "android_dirent.h"
#include <string.h>

android_dirent_t *android_readdir(DIR *dirp) {
    static android_dirent_t ret;
    struct dirent *tmp = readdir(dirp);
    if (tmp) {
        memcpy(&ret.d_name[0], &tmp->d_name[0], 256);
        ret.d_ino = tmp->d_ino;
        ret.d_reclen = tmp->d_reclen;
        ret.d_type = 4;
        ret.d_off = -1;
        return &ret;
    }
    return NULL;
}