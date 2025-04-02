#include "android_posix_types.h"

#ifdef _WIN32
void fd_set_to_native(android_fd_set_t *android_set, fd_set *native_set) {
    int fd;
    FD_ZERO(native_set);
    for (fd = 0; fd < ANDROID_FD_SETSIZE; ++fd) {
        if (ANDROID_FD_ISSET(fd, android_set)) {
            FD_SET(fd, native_set);
        }
    }
}

void fd_set_to_android(fd_set *native_set, android_fd_set_t *android_set) {
    unsigned int i;

    ANDROID_FD_ZERO(android_set);
    for (i = 0; i < native_set->fd_count; ++i) {
        int fd = native_set->fd_array[i];
        if (FD_ISSET(fd, native_set)) {
            ANDROID_FD_SET(fd, android_set);
        }
    }
}
#endif
