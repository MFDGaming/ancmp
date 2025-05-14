#ifndef ANCMP_ANDROID_SOCKET_H
#define ANCMP_ANDROID_SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#include <io.h>
#ifndef AF_BLUETOOTH
#ifdef AF_BTH
#define AF_BLUETOOTH AF_BTH
#endif
#endif
#else
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#define ANDROID_AF_UNSPEC 0
#define ANDROID_AF_UNIX 1  
#define ANDROID_AF_LOCAL 1  
#define ANDROID_AF_INET 2  
#define ANDROID_AF_AX25 3  
#define ANDROID_AF_IPX 4  
#define ANDROID_AF_APPLETALK 5  
#define ANDROID_AF_NETROM 6  
#define ANDROID_AF_BRIDGE 7  
#define ANDROID_AF_ATMPVC 8  
#define ANDROID_AF_X25 9  
#define ANDROID_AF_INET6 10  
#define ANDROID_AF_ROSE 11  
#define ANDROID_AF_DECnet 12  
#define ANDROID_AF_NETBEUI 13  
#define ANDROID_AF_SECURITY 14  
#define ANDROID_AF_KEY 15  
#define ANDROID_AF_NETLINK 16
#define ANDROID_AF_ROUTE AF_NETLINK  
#define ANDROID_AF_PACKET 17  
#define ANDROID_AF_ASH 18  
#define ANDROID_AF_ECONET 19  
#define ANDROID_AF_ATMSVC 20  
#define ANDROID_AF_SNA 22  
#define ANDROID_AF_IRDA 23  
#define ANDROID_AF_PPPOX 24  
#define ANDROID_AF_WANPIPE 25  
#define ANDROID_AF_LLC 26  
#define ANDROID_AF_TIPC 30  
#define ANDROID_AF_BLUETOOTH 31  
#define ANDROID_AF_CAIF 38  
#define ANDROID_AF_MAX 39

#define ANDROID_PF_UNSPEC ANDROID_AF_UNSPEC
#define ANDROID_PF_UNIX ANDROID_AF_UNIX
#define ANDROID_PF_LOCAL ANDROID_AF_LOCAL
#define ANDROID_PF_INET ANDROID_AF_INET
#define ANDROID_PF_AX25 ANDROID_AF_AX25
#define ANDROID_PF_IPX ANDROID_AF_IPX
#define ANDROID_PF_APPLETALK ANDROID_AF_APPLETALK
#define ANDROID_PF_NETROM ANDROID_AF_NETROM
#define ANDROID_PF_BRIDGE ANDROID_AF_BRIDGE
#define ANDROID_PF_ATMPVC ANDROID_AF_ATMPVC
#define ANDROID_PF_X25 ANDROID_AF_X25
#define ANDROID_PF_INET6 ANDROID_AF_INET6
#define ANDROID_PF_ROSE ANDROID_AF_ROSE
#define ANDROID_PF_DECnet ANDROID_AF_DECnet
#define ANDROID_PF_NETBEUI ANDROID_AF_NETBEUI
#define ANDROID_PF_SECURITY ANDROID_AF_SECURITY
#define ANDROID_PF_KEY ANDROID_AF_KEY
#define ANDROID_PF_NETLINK ANDROID_AF_NETLINK
#define ANDROID_PF_ROUTE ANDROID_AF_ROUTE
#define ANDROID_PF_PACKET ANDROID_AF_PACKET
#define ANDROID_PF_ASH ANDROID_AF_ASH
#define ANDROID_PF_ECONET ANDROID_AF_ECONET
#define ANDROID_PF_ATMSVC ANDROID_AF_ATMSVC
#define ANDROID_PF_SNA ANDROID_AF_SNA
#define ANDROID_PF_IRDA ANDROID_AF_IRDA
#define ANDROID_PF_PPPOX ANDROID_AF_PPPOX
#define ANDROID_PF_WANPIPE ANDROID_AF_WANPIPE
#define ANDROID_PF_LLC ANDROID_AF_LLC
#define ANDROID_PF_TIPC ANDROID_AF_TIPC
#define ANDROID_PF_BLUETOOTH ANDROID_AF_BLUETOOTH
#define ANDROID_PF_CAIF ANDROID_AF_CAIF
#define ANDROID_PF_MAX ANDROID_AF_MAX

#define ANDROID_SOCK_STREAM      1
#define ANDROID_SOCK_DGRAM       2
#define ANDROID_SOCK_RAW         3
#define ANDROID_SOCK_RDM         4
#define ANDROID_SOCK_SEQPACKET   5
#define ANDROID_SOCK_PACKET      10

#define ANDROID_SOL_IP 0
#define ANDROID_SOL_SOCKET 1
#define ANDROID_SOL_TCP 6
#define ANDROID_SOL_UDP 17
#define ANDROID_SOL_IPV6 41
#define ANDROID_SOL_ICMPV6 58
#define ANDROID_SOL_SCTP 132
#define ANDROID_SOL_RAW 255
#define ANDROID_SOL_IPX 256
#define ANDROID_SOL_AX25 257
#define ANDROID_SOL_ATALK 258
#define ANDROID_SOL_NETROM 259
#define ANDROID_SOL_ROSE 260
#define ANDROID_SOL_DECNET 261
#define ANDROID_SOL_X25 262
#define ANDROID_SOL_PACKET 263
#define ANDROID_SOL_ATM 264  
#define ANDROID_SOL_AAL 265  
#define ANDROID_SOL_IRDA 266
#define ANDROID_SOL_NETBEUI 267
#define ANDROID_SOL_LLC 268
#define ANDROID_SOL_DCCP 269
#define ANDROID_SOL_NETLINK 270
#define ANDROID_SOL_TIPC 271

#define ANDROID_SO_DEBUG 1
#define ANDROID_SO_REUSEADDR 2
#define ANDROID_SO_TYPE 3
#define ANDROID_SO_ERROR 4
#define ANDROID_SO_DONTROUTE 5
#define ANDROID_SO_BROADCAST 6
#define ANDROID_SO_SNDBUF 7
#define ANDROID_SO_RCVBUF 8
#define ANDROID_SO_SNDBUFFORCE 32
#define ANDROID_SO_RCVBUFFORCE 33
#define ANDROID_SO_KEEPALIVE 9
#define ANDROID_SO_OOBINLINE 10
#define ANDROID_SO_NO_CHECK 11
#define ANDROID_SO_PRIORITY 12
#define ANDROID_SO_LINGER 13
#define ANDROID_SO_BSDCOMPAT 14
#define ANDROID_SO_PASSCRED 16
#define ANDROID_SO_PEERCRED 17
#define ANDROID_SO_RCVLOWAT 18
#define ANDROID_SO_SNDLOWAT 19
#define ANDROID_SO_RCVTIMEO 20
#define ANDROID_SO_SNDTIMEO 21
#define ANDROID_SO_SECURITY_AUTHENTICATION 22
#define ANDROID_SO_SECURITY_ENCRYPTION_TRANSPORT 23
#define ANDROID_SO_SECURITY_ENCRYPTION_NETWORK 24
#define ANDROID_SO_BINDTODEVICE 25
#define ANDROID_SO_ATTACH_FILTER 26
#define ANDROID_SO_DETACH_FILTER 27
#define ANDROID_SO_PEERNAME 28
#define ANDROID_SO_TIMESTAMP 29
#define ANDROID_SO_ACCEPTCONN 30
#define ANDROID_SO_PEERSEC 31
#define ANDROID_SO_PASSSEC 34
#define ANDROID_SO_TIMESTAMPNS 35

#define ANDROID_SCM_RIGHTS 0x01  
#define ANDROID_SCM_CREDENTIALS 0x02  
#define ANDROID_SCM_SECURITY 0x03  
#define ANDROID_SCM_TIMESTAMP ANDROID_SO_TIMESTAMP
#define ANDROID_SCM_TIMESTAMPNS ANDROID_SO_TIMESTAMPNS

#define ANDROID_MSG_OOB 1
#define ANDROID_MSG_PEEK 2
#define ANDROID_MSG_DONTROUTE 4
#define ANDROID_MSG_TRYHARD 4  
#define ANDROID_MSG_CTRUNC 8
#define ANDROID_MSG_PROBE 0x10  
#define ANDROID_MSG_TRUNC 0x20
#define ANDROID_MSG_DONTWAIT 0x40  
#define ANDROID_MSG_EOR 0x80  
#define ANDROID_MSG_WAITALL 0x100  
#define ANDROID_MSG_FIN 0x200
#define ANDROID_MSG_SYN 0x400
#define ANDROID_MSG_CONFIRM 0x800  
#define ANDROID_MSG_RST 0x1000
#define ANDROID_MSG_ERRQUEUE 0x2000  
#define ANDROID_MSG_NOSIGNAL 0x4000  
#define ANDROID_MSG_MORE 0x8000  
#define ANDROID_MSG_EOF MSG_FIN
#define ANDROID_MSG_CMSG_COMPAT 0  

enum {
    ANDROID_IPPROTO_IP = 0,
    ANDROID_IPPROTO_ICMP = 1,
    ANDROID_IPPROTO_IGMP = 2,
    ANDROID_IPPROTO_IPIP = 4,
    ANDROID_IPPROTO_TCP = 6,
    ANDROID_IPPROTO_EGP = 8,
    ANDROID_IPPROTO_PUP = 12,
    ANDROID_IPPROTO_UDP = 17,
    ANDROID_IPPROTO_IDP = 22,
    ANDROID_IPPROTO_DCCP = 33,
    ANDROID_IPPROTO_RSVP = 46,
    ANDROID_IPPROTO_GRE = 47,
    ANDROID_IPPROTO_IPV6 = 41,
    ANDROID_IPPROTO_ESP = 50,
    ANDROID_IPPROTO_AH = 51,
    ANDROID_IPPROTO_PIM = 103,
    ANDROID_IPPROTO_COMP = 108,
    ANDROID_IPPROTO_SCTP = 132,
    ANDROID_IPPROTO_RAW = 255,
    ANDROID_IPPROTO_MAX
   };

#define ANDROID_BTPROTO_L2CAP	0
#define ANDROID_BTPROTO_HCI	1
#define ANDROID_BTPROTO_SCO	2
#define ANDROID_BTPROTO_RFCOMM	3
#define ANDROID_BTPROTO_BNEP	4
#define ANDROID_BTPROTO_CMTP	5
#define ANDROID_BTPROTO_HIDP	6
#define ANDROID_BTPROTO_AVDTP	7

#define ANDROID_SOMAXCONN 128

typedef int android_socklen_t;

typedef unsigned short android_sa_family_t;

typedef struct {
    android_sa_family_t sa_family;
    char sa_data[14];
} android_sockaddr_t;

int af_to_native(int domain);

int af_to_android(int domain);

int sock_to_native(int type);

int sock_to_android(int type);

int ipproto_to_native(int protocol);

int ipproto_to_android(int protocol);

int so_to_native(int option_name);

int sol_to_native(int level);

int msg_to_native(int flags);

int android_socket(int domain, int type, int protocol);

int android_bind(int sockfd, const struct sockaddr *addr, android_socklen_t addrlen);

int android_accept(int sockfd, struct sockaddr *addr, android_socklen_t *addrlen);

int android_connect(int sockfd, const struct sockaddr *addr, android_socklen_t addrlen);

int android_setsockopt(int sockfd, int level, int optname, const void *optval, android_socklen_t optlen);

int android_getsockopt(int sockfd, int level, int optname, void *optval, android_socklen_t *optlen);

long android_recv(int sockfd, void *buf, size_t len, int flags);

long android_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, android_socklen_t *addrlen);

long android_send(int sockfd, const void *buf, size_t len, int flags);

long android_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, android_socklen_t addrlen);

int android_listen(int sockfd, int backlog);

#ifdef _WIN32
int android_close(int fd);

long android_read(int fd, void *buf, size_t count);

long android_write(int fd, void *buf, size_t count);
#else
#define android_close close

#define android_read read

#define android_write write
#endif

#endif
