#ifndef ANCMP_ANDROID_IOCTL_H
#define ANCMP_ANDROID_IOCTL_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#endif

#define ANDROID_SIOCGIFNETMASK 0x891b
#define ANDROID_SIOCGIFCONF    0x8912
#define ANDROID_IFHWADDRLEN 6
#define ANDROID_IFNAMSIZ 16

typedef struct {
    unsigned int type;
    unsigned int size;
    union {
        void *raw_hdlc;
        void *cisco;
        void *fr;
        void *fr_pvc;
        void *fr_pvc_info;
        void *sync;
        void *te1;
    } ifs_ifsu;
} android_if_settings_t;

typedef struct {
    unsigned long mem_start;
    unsigned long mem_end;
    unsigned short base_addr;
    unsigned char irq;
    unsigned char dma;
    unsigned char port;
} android_ifmap_t;

typedef struct {
    union {
        char ifrn_name[ANDROID_IFNAMSIZ];
    } ifr_ifrn;
    union {
        struct sockaddr ifru_addr;
        struct sockaddr ifru_dstaddr;
        struct sockaddr ifru_broadaddr;
        struct sockaddr ifru_netmask;
        struct sockaddr ifru_hwaddr;
        short ifru_flags;
        int ifru_ivalue;
        int ifru_mtu;
        android_ifmap_t ifru_map;
        char ifru_slave[ANDROID_IFNAMSIZ];
        char ifru_newname[ANDROID_IFNAMSIZ];
        void *ifru_data;
        android_if_settings_t ifru_settings;
    } ifr_ifru;
} android_ifreq_t;

typedef struct {
    int ifc_len;
    union {
        char *ifcu_buf;
        android_ifreq_t *ifcu_req;
    } ifc_ifcu;
} android_ifconf_t;

#ifdef _WIN32
int android_ioctl(int fd, unsigned long request, ...);
#else
#define android_ioctl ioctl
#endif

#endif
