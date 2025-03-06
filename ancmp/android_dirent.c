#include "android_dirent.h"
#include <string.h>

android_dirent_t *android_readdir(DIR *dirp) {
    static android_dirent_t ret;
    struct dirent *tmp = readdir(dirp);
    memcpy(ret.d_name, tmp->d_name, (sizeof(ret.d_name) <= sizeof(tmp->d_name)) ? sizeof(ret.d_name) : sizeof(tmp->d_name));
    ret.d_ino = tmp->d_ino;
    ret.d_type = tmp->d_type;
    ret.d_reclen = tmp->d_reclen;
#ifdef _WIN32
    ret.d_off = tmp->d_namlen;
#else
    ret.d_off = tmp->d_off;
#endif
    return &ret;
}