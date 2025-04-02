#include "android_ioctl.h"
#include "android_socket.h"

#ifdef _WIN32

static int io_conf(int sockfd, android_ifconf_t *conf) {
    size_t buffer_length = (conf->ifc_len / sizeof(android_ifreq_t)) * sizeof(INTERFACE_INFO);
    INTERFACE_INFO *buffer = (INTERFACE_INFO *)malloc(buffer_length);
    int interface_count;
    int i;
    unsigned long byte_count = 0;

    if (buffer == NULL) {
        return -1;
    }
    if (WSAIoctl(sockfd, SIO_GET_INTERFACE_LIST, 0, 0, (void *)buffer, buffer_length, &byte_count, 0, 0) == SOCKET_ERROR) {
        return -1;
    }
    interface_count = byte_count / sizeof(INTERFACE_INFO);
    for (i = 0; i < interface_count; i++) {
        char *name = inet_ntoa(buffer[i].iiAddress.AddressIn.sin_addr);
        size_t name_length = strlen(name) + 1;
        memcpy(conf->ifc_ifcu.ifcu_req->ifr_ifrn.ifrn_name, name, (name_length <= ANDROID_IFNAMSIZ) ? name_length : ANDROID_IFNAMSIZ);
        memcpy(&(conf->ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr), &(buffer[i].iiAddress.Address), sizeof(struct sockaddr));
        conf->ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr.sa_family = af_to_android(conf->ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr.sa_family);
    }
    free(buffer);
    return 0;
}

static int io_req(int sockfd, android_ifreq_t *req) {
    INTERFACE_INFO buffer[30];
    unsigned long byte_count = 0;
    int interface_count;
    int i;

    if (WSAIoctl(sockfd, SIO_GET_INTERFACE_LIST, 0, 0, (void *)buffer, sizeof(buffer), &byte_count, 0, 0) == SOCKET_ERROR) {
        return -1;
    }
    interface_count = byte_count / sizeof(INTERFACE_INFO);
    for (i = 0; i < interface_count; i++) {
        char *name = inet_ntoa(buffer[i].iiAddress.AddressIn.sin_addr);
        size_t name_length = strlen(name) + 1;
        if (strncmp(name, req->ifr_ifrn.ifrn_name, (name_length <= ANDROID_IFNAMSIZ) ? name_length : ANDROID_IFNAMSIZ) == 0) {
            memcpy(&(req->ifr_ifru.ifru_netmask), &(buffer[i].iiNetmask.Address), sizeof(struct sockaddr));
            req->ifr_ifru.ifru_netmask.sa_family = af_to_android(req->ifr_ifru.ifru_netmask.sa_family);
            return 0;
        }
    }
    return -1;
}

int android_ioctl(int fd, unsigned long request, ...) {
    va_list args;
    va_start(args, request);
    if (request == ANDROID_SIOCGIFCONF) {
        android_ifconf_t *conf = (android_ifconf_t *)va_arg(args, android_ifconf_t *);
        if (io_conf(fd, conf) != -1) {
            return 0;
        }
    } else if (request == ANDROID_SIOCGIFNETMASK) {
        android_ifreq_t *req = (android_ifreq_t *)va_arg(args, android_ifreq_t *);
        if (io_req(fd, req) != -1) {
            return 0;
        }
    }
    return -1;
}

#endif
