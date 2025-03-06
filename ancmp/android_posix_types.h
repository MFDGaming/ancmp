#pragma once

#include <string.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/select.h>
#endif

#define ANDROID_NFDBITS (8 * sizeof(unsigned long))
#define ANDROID_FD_SETSIZE 1024
#define ANDROID_FDSET_LONGS (ANDROID_FD_SETSIZE/ANDROID_NFDBITS)
#define ANDROID_FDELT(d) ((d) / ANDROID_NFDBITS)
#define ANDROID_FDMASK(d) (1UL << ((d) % ANDROID_NFDBITS))

typedef struct {
    unsigned long fds_bits [ANDROID_FDSET_LONGS];
} android_fd_set_t;

#define ANDROID_FD_SET(fd, fdsetp) \
    ((fdsetp)->fds_bits[ANDROID_FDELT(fd)] |= ANDROID_FDMASK(fd))

#define ANDROID_FD_CLR(fd, fdsetp) \
    ((fdsetp)->fds_bits[ANDROID_FDELT(fd)] &= ~ANDROID_FDMASK(fd))

#define ANDROID_FD_ISSET(fd, fdsetp) \
    (((fdsetp)->fds_bits[ANDROID_FDELT(fd)] & ANDROID_FDMASK(fd)) != 0)

#define ANDROID_FD_ZERO(fdsetp) \
    (memset((void *)(fdsetp), 0, sizeof(android_fd_set_t)))

#ifdef _WIN32
void fd_set_to_native(android_fd_set_t *android_set, fd_set *native_set);

void fd_set_to_android(fd_set *native_set, android_fd_set_t *android_set);
#endif