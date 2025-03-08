#include "android_dirent.h"
#include <string.h>

android_dirent_t *android_readdir(DIR *dirp) {
    static android_dirent_t ret;
    struct dirent *tmp = readdir(dirp);
    if (tmp) {
        memcpy(&ret.d_name[0], &tmp->d_name[0], 256);
        return &ret;
    }
    return NULL;
}