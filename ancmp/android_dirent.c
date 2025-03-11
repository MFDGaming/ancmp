#include "android_dirent.h"
#include <string.h>
#include <errno.h>

android_dirent_t *android_readdir(DIR *dirp) {
    static android_dirent_t ret;
    struct dirent *tmp = readdir(dirp);
    if (tmp) {
        memcpy(ret.d_name, tmp->d_name, 256);
        ret.d_ino = tmp->d_ino;
        ret.d_reclen = tmp->d_reclen;
#ifdef _WIN32
        ret.d_type = 4;
        ret.d_off = -1;
#else
        ret.d_type = tmp->d_type;
        ret.d_off = tmp->d_off;
#endif
        ret.d_name[255] = '\0';
        return &ret;
    }
    return NULL;
}