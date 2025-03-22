#include "android_dirent.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>

#ifdef _WIN32

android_DIR *android_opendir(const char *name) {
    char path[MAX_PATH];
    snprintf(path, sizeof(path), "%s\\*", name);

    android_DIR *dir = (android_DIR *)malloc(sizeof(android_DIR));
    if (dir == NULL) {
        return NULL;
    }

    dir->hFind = FindFirstFile(path, &(dir->findFileData));
    if (dir->hFind == INVALID_HANDLE_VALUE) {
        free(dir);
        return NULL;
    }

    return dir;
}

int android_closedir(android_DIR *dirp) {
    if (dirp == NULL) {
        return -1;
    }

    int result = FindClose(dirp->hFind);
    free(dirp);
    return result ? 0 : -1;
}

android_dirent_t *android_readdir(android_DIR *dirp) {
    if (dirp == NULL || dirp->hFind == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    android_dirent_t *entry = (android_dirent_t *)malloc(sizeof(android_dirent_t));
    if (entry == NULL) {
        return NULL;
    }

    entry->d_ino = 0;
    entry->d_off = 0;
    entry->d_reclen = sizeof(android_dirent_t);
    entry->d_type = (dirp->findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 4 : 8;

    strncpy(entry->d_name, dirp->findFileData.cFileName, sizeof(entry->d_name) - 1);
    entry->d_name[sizeof(entry->d_name) - 1] = '\0';

    if (FindNextFile(dirp->hFind, &(dirp->findFileData)) == 0) {
        return NULL;
    }

    return entry;
}

#else

android_dirent_t *android_readdir(DIR *dirp) {
    static android_dirent_t ret;
    struct dirent *tmp = readdir(dirp);
    if (tmp) {
        memcpy(ret.d_name, tmp->d_name, (sizeof(tmp->d_name) > 256) ? 256 : sizeof(tmp->d_name));
        ret.d_ino = tmp->d_ino;
        ret.d_reclen = sizeof(android_dirent_t);
        ret.d_type = tmp->d_type;
        ret.d_off = tmp->d_off;
        ret.d_name[255] = '\0';
        return &ret;
    }
    return NULL;
}

#endif