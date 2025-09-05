#include "android_socket.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "android_posix_types.h"
#include "android_fcntl.h"

int af_to_native(int domain) {
#ifdef AF_UNSPEC
    if (domain == ANDROID_AF_UNSPEC) {
        return AF_UNSPEC;
    }
#endif
#ifdef AF_UNIX
    if (domain == ANDROID_AF_UNIX) {
        return AF_UNIX;
    }
#endif
#ifdef AF_LOCAL
    if (domain == ANDROID_AF_LOCAL) {
        return AF_LOCAL;
    }
#endif
#ifdef AF_INET
    if (domain == ANDROID_AF_INET) {
        return AF_INET;
    }
#endif
#ifdef AF_AX25
    if (domain == ANDROID_AF_AX25) {
        return AF_AX25;
    }
#endif
#ifdef AF_IPX
    if (domain == ANDROID_AF_IPX) {
        return AF_IPX;
    }
#endif
#ifdef AF_APPLETALK
    if (domain == ANDROID_AF_APPLETALK) {
        return AF_APPLETALK;
    }
#endif
#ifdef AF_NETROM
    if (domain == ANDROID_AF_NETROM) {
        return AF_NETROM;
    }
#endif
#ifdef AF_BRIDGE
    if (domain == ANDROID_AF_BRIDGE) {
        return AF_BRIDGE;
    }
#endif
#ifdef AF_ATMPVC
    if (domain == ANDROID_AF_ATMPVC) {
        return AF_ATMPVC;
    }
#endif
#ifdef AF_X25
    if (domain == ANDROID_AF_X25) {
        return AF_X25;
    }
#endif
#ifdef AF_INET6
    if (domain == ANDROID_AF_INET6) {
        return AF_INET6;
    }
#endif
#ifdef AF_ROSE
    if (domain == ANDROID_AF_ROSE) {
        return AF_ROSE;
    }
#endif
#ifdef AF_DECnet
    if (domain == ANDROID_AF_DECnet) {
        return AF_DECnet;
    }
#endif
#ifdef AF_NETBEUI
    if (domain == ANDROID_AF_NETBEUI) {
        return AF_NETBEUI;
    }
#endif
#ifdef AF_SECURITY
    if (domain == ANDROID_AF_SECURITY) {
        return AF_SECURITY;
    }
#endif
#ifdef AF_KEY
    if (domain == ANDROID_AF_KEY) {
        return AF_KEY;
    }
#endif
#ifdef AF_NETLINK
    if (domain == ANDROID_AF_NETLINK) {
        return AF_NETLINK;
    }
#endif
#ifdef AF_ROUTE
    if (domain == ANDROID_AF_ROUTE) {
        return AF_ROUTE;
    }
#endif
#ifdef AF_PACKET
    if (domain == ANDROID_AF_PACKET) {
        return AF_PACKET;
    }
#endif
#ifdef AF_ASH
    if (domain == ANDROID_AF_ASH) {
        return AF_ASH;
    }
#endif
#ifdef AF_ECONET
    if (domain == ANDROID_AF_ECONET) {
        return AF_ECONET;
    }
#endif
#ifdef AF_ATMSVC
    if (domain == ANDROID_AF_ATMSVC) {
        return AF_ATMSVC;
    }
#endif
#ifdef AF_SNA
    if (domain == ANDROID_AF_SNA) {
        return AF_SNA;
    }
#endif
    #ifdef AF_IRDA
    if (domain == ANDROID_AF_IRDA) {
        return AF_IRDA;
    }
#endif
#ifdef AF_PPPOX
    if (domain == ANDROID_AF_PPPOX) {
        return AF_PPPOX;
    }
#endif
#ifdef AF_WANPIPE
    if (domain == ANDROID_AF_WANPIPE) {
        return AF_WANPIPE;
    }
#endif
#ifdef AF_LLC
    if (domain == ANDROID_AF_LLC) {
        return AF_LLC;
    }
#endif
#ifdef AF_TIPC
    if (domain == ANDROID_AF_TIPC) {
        return AF_TIPC;
    }
#endif
#ifdef AF_BLUETOOTH
    if (domain == ANDROID_AF_BLUETOOTH) {
        return AF_BLUETOOTH;
    }
#endif
#ifdef AF_CAIF
    if (domain == ANDROID_AF_CAIF) {
        return AF_CAIF;
    }
#endif
    return -1;
}

int af_to_android(int domain) {
#ifdef AF_UNSPEC
    if (domain == AF_UNSPEC) {
        return ANDROID_AF_UNSPEC;
    }
#endif
#ifdef AF_UNIX
    if (domain == AF_UNIX) {
        return ANDROID_AF_UNIX;
    }
#endif
#ifdef AF_LOCAL
    if (domain == AF_LOCAL) {
        return ANDROID_AF_LOCAL;
    }
#endif
#ifdef AF_INET
    if (domain == AF_INET) {
        return ANDROID_AF_INET;
    }
#endif
#ifdef AF_AX25
    if (domain == AF_AX25) {
        return ANDROID_AF_AX25;
    }
#endif
#ifdef AF_IPX
    if (domain == AF_IPX) {
        return ANDROID_AF_IPX;
    }
#endif
#ifdef AF_APPLETALK
    if (domain == AF_APPLETALK) {
        return ANDROID_AF_APPLETALK;
    }
#endif
#ifdef AF_NETROM
    if (domain == AF_NETROM) {
        return ANDROID_AF_NETROM;
    }
#endif
#ifdef AF_BRIDGE
    if (domain == AF_BRIDGE) {
        return ANDROID_AF_BRIDGE;
    }
#endif
#ifdef AF_ATMPVC
    if (domain == AF_ATMPVC) {
        return ANDROID_AF_ATMPVC;
    }
#endif
#ifdef AF_X25
    if (domain == AF_X25) {
        return ANDROID_AF_X25;
    }
#endif
#ifdef AF_INET6
    if (domain == AF_INET6) {
        return ANDROID_AF_INET6;
    }
#endif
#ifdef AF_ROSE
    if (domain == AF_ROSE) {
        return ANDROID_AF_ROSE;
    }
#endif
#ifdef AF_DECnet
    if (domain == AF_DECnet) {
        return ANDROID_AF_DECnet;
    }
#endif
#ifdef AF_NETBEUI
    if (domain == AF_NETBEUI) {
        return ANDROID_AF_NETBEUI;
    }
#endif
#ifdef AF_SECURITY
    if (domain == AF_SECURITY) {
        return ANDROID_AF_SECURITY;
    }
#endif
#ifdef AF_KEY
    if (domain == AF_KEY) {
        return ANDROID_AF_KEY;
    }
#endif
#ifdef AF_NETLINK
    if (domain == AF_NETLINK) {
        return ANDROID_AF_NETLINK;
    }
#endif
#ifdef AF_ROUTE
    if (domain == AF_ROUTE) {
        return ANDROID_AF_ROUTE;
    }
#endif
#ifdef AF_PACKET
    if (domain == AF_PACKET) {
        return ANDROID_AF_PACKET;
    }
#endif
#ifdef AF_ASH
    if (domain == AF_ASH) {
        return ANDROID_AF_ASH;
    }
#endif
#ifdef AF_ECONET
    if (domain == AF_ECONET) {
        return ANDROID_AF_ECONET;
    }
#endif
#ifdef AF_ATMSVC
    if (domain == AF_ATMSVC) {
        return ANDROID_AF_ATMSVC;
    }
#endif
#ifdef AF_SNA
    if (domain == AF_SNA) {
        return ANDROID_AF_SNA;
    }
#endif
#ifdef AF_IRDA
    if (domain == AF_IRDA) {
        return ANDROID_AF_IRDA;
    }
#endif
#ifdef AF_PPPOX
    if (domain == AF_PPPOX) {
        return ANDROID_AF_PPPOX;
    }
#endif
#ifdef AF_WANPIPE
    if (domain == AF_WANPIPE) {
        return ANDROID_AF_WANPIPE;
    }
#endif
#ifdef AF_LLC
    if (domain == AF_LLC) {
        return ANDROID_AF_LLC;
    }
#endif
#ifdef AF_TIPC
    if (domain == AF_TIPC) {
        return ANDROID_AF_TIPC;
    }
#endif
#ifdef AF_BLUETOOTH
    if (domain == AF_BLUETOOTH) {
        return ANDROID_AF_BLUETOOTH;
    }
#endif
#ifdef AF_CAIF
    if (domain == AF_CAIF) {
        return ANDROID_AF_CAIF;
    }
#endif
    return -1;
}

int sock_to_native(int type) {
#ifdef SOCK_STREAM
    if (type == ANDROID_SOCK_STREAM) {
        return SOCK_STREAM;
    }
#endif
#ifdef SOCK_DGRAM
    if (type == ANDROID_SOCK_DGRAM) {
        return SOCK_DGRAM;
    }
#endif
#ifdef SOCK_RAW
    if (type == ANDROID_SOCK_RAW) {
        return SOCK_RAW;
    }
#endif
#ifdef SOCK_RDM
    if (type == ANDROID_SOCK_RDM) {
        return SOCK_RDM;
    }
#endif
#ifdef SOCK_SEQPACKET
    if (type == ANDROID_SOCK_SEQPACKET) {
        return SOCK_SEQPACKET;
    }
#endif
#ifdef SOCK_PACKET
    if (type == ANDROID_SOCK_PACKET) {
        return SOCK_PACKET;
    }
#endif
    return -1;
}

int sock_to_android(int type) {
#ifdef SOCK_STREAM
    if (type == SOCK_STREAM) {
        return ANDROID_SOCK_STREAM;
    }
#endif
#ifdef SOCK_DGRAM
    if (type == SOCK_DGRAM) {
        return ANDROID_SOCK_DGRAM;
    }
#endif
#ifdef SOCK_RAW
    if (type == SOCK_RAW) {
        return ANDROID_SOCK_RAW;
    }
#endif
#ifdef SOCK_RDM
    if (type == SOCK_RDM) {
        return ANDROID_SOCK_RDM;
    }
#endif
#ifdef SOCK_SEQPACKET
    if (type == SOCK_SEQPACKET) {
        return ANDROID_SOCK_SEQPACKET;
    }
#endif
#ifdef SOCK_PACKET
    if (type == SOCK_PACKET) {
        return ANDROID_SOCK_PACKET;
    }
#endif
    return -1;
}

int ipproto_to_native(int protocol) {
#ifdef IPPROTO_IP
    if (protocol == ANDROID_IPPROTO_IP) {
        return IPPROTO_IP;
    }
#endif
#ifdef IPPROTO_ICMP
    if (protocol == ANDROID_IPPROTO_ICMP) {
        return IPPROTO_ICMP;
    }
#endif
#ifdef IPPROTO_IGMP
    if (protocol == ANDROID_IPPROTO_IGMP) {
        return IPPROTO_IGMP;
    }
    #endif
#ifdef IPPROTO_IPIP
    if (protocol == ANDROID_IPPROTO_IPIP) {
        return IPPROTO_IPIP;
    }
#endif
#ifdef IPPROTO_TCP
    if (protocol == ANDROID_IPPROTO_TCP) {
        return IPPROTO_TCP;
    }
#endif
#ifdef IPPROTO_EGP
    if (protocol == ANDROID_IPPROTO_EGP) {
        return IPPROTO_EGP;
    }
#endif
#ifdef IPPROTO_PUP
    if (protocol == ANDROID_IPPROTO_PUP) {
        return IPPROTO_PUP;
    }
#endif
#ifdef IPPROTO_UDP
    if (protocol == ANDROID_IPPROTO_UDP) {
        return IPPROTO_UDP;
    }
#endif
#ifdef IPPROTO_IDP
    if (protocol == ANDROID_IPPROTO_IDP) {
        return IPPROTO_IDP;
    }
#endif
#ifdef IPPROTO_DCCP
    if (protocol == ANDROID_IPPROTO_DCCP) {
        return IPPROTO_DCCP;
    }
#endif
#ifdef IPPROTO_RSVP
    if (protocol == ANDROID_IPPROTO_RSVP) {
        return IPPROTO_RSVP;
    }
#endif
#ifdef IPPROTO_GRE
    if (protocol == ANDROID_IPPROTO_GRE) {
        return IPPROTO_GRE;
    }
#endif
#ifdef IPPROTO_IPV6
    if (protocol == ANDROID_IPPROTO_IPV6) {
        return IPPROTO_IPV6;
    }
#endif
#ifdef IPPROTO_ESP
    if (protocol == ANDROID_IPPROTO_ESP) {
        return IPPROTO_ESP;
    }
#endif
#ifdef IPPROTO_AH
    if (protocol == ANDROID_IPPROTO_AH) {
        return IPPROTO_AH;
    }
#endif
#ifdef IPPROTO_PIM
    if (protocol == ANDROID_IPPROTO_PIM) {
        return IPPROTO_PIM;
    }
#endif
#ifdef IPPROTO_COMP
    if (protocol == ANDROID_IPPROTO_COMP) {
        return IPPROTO_COMP;
    }
#endif
#ifdef IPPROTO_SCTP
    if (protocol == ANDROID_IPPROTO_SCTP) {
        return IPPROTO_SCTP;
    }
#endif
#ifdef IPPROTO_RAW
    if (protocol == ANDROID_IPPROTO_RAW) {
        return IPPROTO_RAW;
    }
#endif
    return -1;
}

int ipproto_to_android(int protocol) {
#ifdef IPPROTO_IP
    if (protocol == IPPROTO_IP) {
        return ANDROID_IPPROTO_IP;
    }
#endif
#ifdef IPPROTO_ICMP
    if (protocol == IPPROTO_ICMP) {
        return ANDROID_IPPROTO_ICMP;
    }
#endif
#ifdef IPPROTO_IGMP
    if (protocol == IPPROTO_IGMP) {
        return ANDROID_IPPROTO_IGMP;
    }
    #endif
#ifdef IPPROTO_IPIP
    if (protocol == IPPROTO_IPIP) {
        return ANDROID_IPPROTO_IPIP;
    }
#endif
#ifdef IPPROTO_TCP
    if (protocol == IPPROTO_TCP) {
        return ANDROID_IPPROTO_TCP;
    }
#endif
#ifdef IPPROTO_EGP
    if (protocol == IPPROTO_EGP) {
        return ANDROID_IPPROTO_EGP;
    }
#endif
#ifdef IPPROTO_PUP
    if (protocol == IPPROTO_PUP) {
        return ANDROID_IPPROTO_PUP;
    }
#endif
#ifdef IPPROTO_UDP
    if (protocol == IPPROTO_UDP) {
        return ANDROID_IPPROTO_UDP;
    }
#endif
#ifdef IPPROTO_IDP
    if (protocol == IPPROTO_IDP) {
        return ANDROID_IPPROTO_IDP;
    }
#endif
#ifdef IPPROTO_DCCP
    if (protocol == IPPROTO_DCCP) {
        return ANDROID_IPPROTO_DCCP;
    }
#endif
#ifdef IPPROTO_RSVP
    if (protocol == IPPROTO_RSVP) {
        return ANDROID_IPPROTO_RSVP;
    }
#endif
#ifdef IPPROTO_GRE
    if (protocol == IPPROTO_GRE) {
        return ANDROID_IPPROTO_GRE;
    }
#endif
#ifdef IPPROTO_IPV6
    if (protocol == IPPROTO_IPV6) {
        return ANDROID_IPPROTO_IPV6;
    }
#endif
#ifdef IPPROTO_ESP
    if (protocol == IPPROTO_ESP) {
        return ANDROID_IPPROTO_ESP;
    }
#endif
#ifdef IPPROTO_AH
    if (protocol == IPPROTO_AH) {
        return ANDROID_IPPROTO_AH;
    }
#endif
#ifdef IPPROTO_PIM
    if (protocol == IPPROTO_PIM) {
        return ANDROID_IPPROTO_PIM;
    }
#endif
#ifdef IPPROTO_COMP
    if (protocol == IPPROTO_COMP) {
        return ANDROID_IPPROTO_COMP;
    }
#endif
#ifdef IPPROTO_SCTP
    if (protocol == IPPROTO_SCTP) {
        return ANDROID_IPPROTO_SCTP;
    }
#endif
#ifdef IPPROTO_RAW
    if (protocol == IPPROTO_RAW) {
        return ANDROID_IPPROTO_RAW;
    }
#endif
    return -1;
}

int so_to_native(int option_name) {
#ifdef SO_DEBUG
    if (option_name == ANDROID_SO_DEBUG) {
        return SO_DEBUG;
    }
#endif
#ifdef SO_REUSEADDR
    if (option_name == ANDROID_SO_REUSEADDR) {
        return SO_REUSEADDR;
    }
#endif
#ifdef SO_TYPE
    if (option_name == ANDROID_SO_TYPE) {
        return SO_TYPE;
    }
#endif
#ifdef SO_ERROR
    if (option_name == ANDROID_SO_ERROR) {
        return SO_ERROR;
    }
#endif
#ifdef SO_DONTROUTE
    if (option_name == ANDROID_SO_DONTROUTE) {
        return SO_DONTROUTE;
    }
#endif
#ifdef SO_BROADCAST
    if (option_name == ANDROID_SO_BROADCAST) {
        return SO_BROADCAST;
    }
#endif
#ifdef SO_SNDBUF
    if (option_name == ANDROID_SO_SNDBUF) {
        return SO_SNDBUF;
    }
#endif
#ifdef SO_RCVBUF
    if (option_name == ANDROID_SO_RCVBUF) {
        return SO_RCVBUF;
    }
#endif
#ifdef SO_SNDBUFFORCE
    if (option_name == ANDROID_SO_SNDBUFFORCE) {
        return SO_SNDBUFFORCE;
    }
#endif
#ifdef SO_RCVBUFFORCE
    if (option_name == ANDROID_SO_RCVBUFFORCE) {
        return SO_RCVBUFFORCE;
    }
#endif
#ifdef SO_KEEPALIVE
    if (option_name == ANDROID_SO_KEEPALIVE) {
        return SO_KEEPALIVE;
    }
#endif
#ifdef SO_OOBINLINE
    if (option_name == ANDROID_SO_OOBINLINE) {
        return SO_OOBINLINE;
    }
#endif
#ifdef SO_NO_CHECK
    if (option_name == ANDROID_SO_NO_CHECK) {
        return SO_NO_CHECK;
    }
#endif
#ifdef SO_PRIORITY
    if (option_name == ANDROID_SO_PRIORITY) {
        return SO_PRIORITY;
    }
#endif
#ifdef SO_LINGER
    if (option_name == ANDROID_SO_LINGER) {
        return SO_LINGER;
    }
#endif
#ifdef SO_BSDCOMPAT
    if (option_name == ANDROID_SO_BSDCOMPAT) {
        return SO_BSDCOMPAT;
    }
#endif
#ifdef SO_PASSCRED
    if (option_name == ANDROID_SO_PASSCRED) {
        return SO_PASSCRED;
    }
#endif
#ifdef SO_PEERCRED
    if (option_name == ANDROID_SO_PEERCRED) {
        return SO_PEERCRED;
    }
#endif
#ifdef SO_RCVLOWAT
    if (option_name == ANDROID_SO_RCVLOWAT) {
        return SO_RCVLOWAT;
    }
#endif
#ifdef SO_SNDLOWAT
    if (option_name == ANDROID_SO_SNDLOWAT) {
        return SO_SNDLOWAT;
    }
#endif
#ifdef SO_RCVTIMEO
    if (option_name == ANDROID_SO_RCVTIMEO) {
        return SO_RCVTIMEO;
    }
#endif
#ifdef SO_SNDTIMEO
    if (option_name == ANDROID_SO_SNDTIMEO) {
        return SO_SNDTIMEO;
    }
#endif
#ifdef SO_SECURITY_AUTHENTICATION
    if (option_name == ANDROID_SO_SECURITY_AUTHENTICATION) {
        return SO_SECURITY_AUTHENTICATION;
    }
#endif
#ifdef SO_SECURITY_ENCRYPTION_TRANSPORT
    if (option_name == ANDROID_SO_SECURITY_ENCRYPTION_TRANSPORT) {
        return SO_SECURITY_ENCRYPTION_TRANSPORT;
    }
#endif
#ifdef SO_SECURITY_ENCRYPTION_NETWORK
    if (option_name == ANDROID_SO_SECURITY_ENCRYPTION_NETWORK) {
        return SO_SECURITY_ENCRYPTION_NETWORK;
    }
#endif
#ifdef SO_BINDTODEVICE
    if (option_name == ANDROID_SO_BINDTODEVICE) {
        return SO_BINDTODEVICE;
    }
#endif
#ifdef SO_ATTACH_FILTER
    if (option_name == ANDROID_SO_ATTACH_FILTER) {
        return SO_ATTACH_FILTER;
    }
#endif
#ifdef SO_DETACH_FILTER
    if (option_name == ANDROID_SO_DETACH_FILTER) {
        return SO_DETACH_FILTER;
    }
#endif
#ifdef SO_PEERNAME
    if (option_name == ANDROID_SO_PEERNAME) {
        return SO_PEERNAME;
    }
#endif
#ifdef SO_TIMESTAMP
    if (option_name == ANDROID_SO_TIMESTAMP) {
        return SO_TIMESTAMP;
    }
#endif
#ifdef SO_TIMESTAMPNS
    if (option_name == ANDROID_SO_TIMESTAMPNS) {
        return SO_TIMESTAMPNS;
    }
#endif
#ifdef SO_ACCEPTCONN
    if (option_name == ANDROID_SO_ACCEPTCONN) {
        return SO_ACCEPTCONN;
    }
#endif
#ifdef SO_PEERSEC
    if (option_name == ANDROID_SO_PEERSEC) {
        return SO_PEERSEC;
    }
#endif
#ifdef SO_PASSSEC
    if (option_name == ANDROID_SO_PASSSEC) {
        return SO_PASSSEC;
    }
#endif
    return -1;
}

int sol_to_native(int level) {
#ifdef SOL_IP
    if (level == ANDROID_SOL_IP) {
        return SOL_IP;
    }
#endif
#ifdef SOL_SOCKET
    if (level == ANDROID_SOL_SOCKET) {
        return SOL_SOCKET;
    }
#endif
#ifdef SOL_TCP
    if (level == ANDROID_SOL_TCP) {
        return SOL_TCP;
    }
#endif
#ifdef SOL_UDP
    if (level == ANDROID_SOL_UDP) {
        return SOL_UDP;
    }
#endif
#ifdef SOL_IPV6
    if (level == ANDROID_SOL_IPV6) {
        return SOL_IPV6;
    }
#endif
#ifdef SOL_ICMPV6
    if (level == ANDROID_SOL_ICMPV6) {
        return SOL_ICMPV6;
    }
#endif
#ifdef SOL_SCTP
    if (level == ANDROID_SOL_SCTP) {
        return SOL_SCTP;
    }
#endif
#ifdef SOL_RAW
    if (level == ANDROID_SOL_RAW) {
        return SOL_RAW;
    }
#endif
#ifdef SOL_IPX
    if (level == ANDROID_SOL_IPX) {
        return SOL_IPX;
    }
#endif
#ifdef SOL_AX25
    if (level == ANDROID_SOL_AX25) {
        return SOL_AX25;
    }
#endif
#ifdef SOL_ATALK
    if (level == ANDROID_SOL_ATALK) {
        return SOL_ATALK;
    }
#endif
#ifdef SOL_NETROM
    if (level == ANDROID_SOL_NETROM) {
        return SOL_NETROM;
    }
#endif
#ifdef SOL_ROSE
    if (level == ANDROID_SOL_ROSE) {
        return SOL_ROSE;
    }
#endif
#ifdef SOL_DECNET
    if (level == ANDROID_SOL_DECNET) {
        return SOL_DECNET;
    }
#endif
#ifdef SOL_X25
    if (level == ANDROID_SOL_X25) {
        return SOL_X25;
    }
#endif
#ifdef SOL_PACKET
    if (level == ANDROID_SOL_PACKET) {
        return SOL_PACKET;
    }
#endif
#ifdef SOL_ATM
    if (level == ANDROID_SOL_ATM) {
        return SOL_ATM;
    }
#endif
#ifdef SOL_AAL
    if (level == ANDROID_SOL_AAL) {
        return SOL_AAL;
    }
#endif
#ifdef SOL_IRDA
    if (level == ANDROID_SOL_IRDA) {
        return SOL_IRDA;
    }
#endif
#ifdef SOL_NETBEUI
    if (level == ANDROID_SOL_NETBEUI) {
        return SOL_NETBEUI;
    }
#endif
#ifdef SOL_LLC
    if (level == ANDROID_SOL_LLC) {
        return SOL_LLC;
    }
#endif
#ifdef SOL_DCCP
    if (level == ANDROID_SOL_DCCP) {
        return SOL_DCCP;
    }
#endif
#ifdef SOL_NETLINK
    if (level == ANDROID_SOL_NETLINK) {
        return SOL_NETLINK;
    }
#endif
#ifdef SOL_TIPC
    if (level == ANDROID_SOL_TIPC) {
        return SOL_TIPC;
    }
#endif
    return -1;
}

int msg_to_native(int flags) {
    int ret = 0;
#ifdef MSG_OOB
    if (flags & ANDROID_MSG_OOB) {
        ret |= MSG_OOB;
    }
#endif
#ifdef MSG_PEEK
    if (flags & ANDROID_MSG_PEEK) {
        ret |= MSG_PEEK;
    }
#endif
#ifdef MSG_DONTROUTE
    if (flags & ANDROID_MSG_DONTROUTE) {
        ret |= MSG_DONTROUTE;
    }
#endif
#ifdef MSG_TRYHARD
    if (flags & ANDROID_MSG_TRYHARD) {
        ret |= MSG_TRYHARD;
    }
#endif
#ifdef MSG_CTRUNC
    if (flags & ANDROID_MSG_CTRUNC) {
        ret |= MSG_CTRUNC;
    }
#endif
#ifdef MSG_PROBE
    if (flags & ANDROID_MSG_PROBE) {
        ret |= MSG_PROBE;
    }
#endif
#ifdef MSG_TRUNC
    if (flags & ANDROID_MSG_TRUNC) {
        ret |= MSG_TRUNC;
    }
#endif
#ifdef MSG_DONTWAIT
    if (flags & ANDROID_MSG_DONTWAIT) {
        ret |= MSG_DONTWAIT;
    }
#endif
#ifdef MSG_EOR
    if (flags & ANDROID_MSG_EOR) {
        ret |= MSG_EOR;
    }
#endif
#ifdef MSG_WAITALL
    if (flags & ANDROID_MSG_WAITALL) {
        ret |= MSG_WAITALL;
    }
#endif
#ifdef MSG_FIN
    if (flags & ANDROID_MSG_FIN) {
        ret |= MSG_FIN;
    }
#endif
#ifdef MSG_SYN
    if (flags & ANDROID_MSG_SYN) {
        ret |= MSG_SYN;
    }
#endif
#ifdef MSG_CONFIRM
    if (flags & ANDROID_MSG_CONFIRM) {
        ret |= MSG_CONFIRM;
    }
#endif
#ifdef MSG_RST
    if (flags & ANDROID_MSG_RST) {
        ret |= MSG_RST;
    }
#endif
#ifdef MSG_ERRQUEUE
    if (flags & ANDROID_MSG_ERRQUEUE) {
        ret |= MSG_ERRQUEUE;
    }
#endif
#ifdef MSG_NOSIGNAL
    if (flags & ANDROID_MSG_NOSIGNAL) {
        ret |= MSG_NOSIGNAL;
    }
#endif
#ifdef MSG_MORE
    if (flags & ANDROID_MSG_MORE) {
        ret |= MSG_MORE;
    }
#endif
    return ret;
}


int android_socket(int domain, int type, int protocol) {
    return socket(af_to_native(domain), sock_to_native(type), ipproto_to_native(protocol));
}

int android_bind(int sockfd, const struct sockaddr *addr, android_socklen_t addrlen) {
    if (addr != NULL) {
        struct sockaddr *a = (struct sockaddr *)malloc(addrlen);
        if (a) {
            int ret;
            
            memcpy(a, addr, addrlen);
            a->sa_family = af_to_native(a->sa_family);
            ret = bind(sockfd, a, addrlen);
            free(a);
            return ret;
        }
    }
    return -1;
}

int android_accept(int sockfd, struct sockaddr *addr, android_socklen_t *addrlen) {
    if (addr != NULL) {
        int ret = accept(sockfd, addr, addrlen);
        *((short *)addr) = af_to_android(*((short *)addr));
        return ret;
    }
    return accept(sockfd, NULL, NULL);
}

int android_connect(int sockfd, const struct sockaddr *addr, android_socklen_t addrlen) {
    if (addr != NULL) {
        struct sockaddr *a = (struct sockaddr *)malloc(addrlen);
        if (a) {
            int ret;

            memcpy(a, addr, addrlen);
            a->sa_family = af_to_native(a->sa_family);
            ret = connect(sockfd, a, addrlen);
            free(a);
            return ret;
        }
    }
    return -1;
}

int android_setsockopt(int sockfd, int level, int optname, const void *optval, android_socklen_t optlen) {
    return setsockopt(sockfd, sol_to_native(level), so_to_native(optname), optval, optlen);
}

int android_getsockopt(int sockfd, int level, int optname, void *optval, android_socklen_t *optlen) {
    return getsockopt(sockfd, sol_to_native(level), so_to_native(optname), optval, optlen);
}

long android_recv(int sockfd, void *buf, size_t len, int flags) {
    return recv(sockfd, buf, len, msg_to_native(flags));
}

long android_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, android_socklen_t *addrlen) {
    if (src_addr != NULL) {
        long ret = recvfrom(sockfd, buf, len, msg_to_native(flags), src_addr, addrlen);
        *((short *)src_addr) = af_to_android(*((short *)src_addr));
        return ret;
    }
    return recvfrom(sockfd, buf, len, msg_to_native(flags), NULL, NULL);
}

long android_send(int sockfd, const void *buf, size_t len, int flags) {
    return send(sockfd, buf, len, msg_to_native(flags));
}

long android_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, android_socklen_t addrlen) {
    if (dest_addr != NULL) {
        struct sockaddr *a = (struct sockaddr *)malloc(addrlen);
        if (a) {
            int ret;

            memcpy(a, dest_addr, addrlen);
            a->sa_family = af_to_native(a->sa_family);
            ret = sendto(sockfd, buf, len, msg_to_native(flags), a, addrlen);
            free(a);
            return ret;
        }
    }
    return -1;
}

int android_listen(int sockfd, int backlog) {
    return listen(sockfd, (backlog == ANDROID_SOMAXCONN) ? SOMAXCONN : 0);
}

#ifdef _WIN32
int android_close(int fd) {
    if (is_socket(fd)) {
        return closesocket(fd);
    } else if (fd >= 0 && fd < _getmaxstdio()) {
        return close(fd);
    }
    return -1;
}

long android_read(int fd, void *buf, size_t count) {
    if (is_socket(fd)) {
        return recv(fd, buf, count, 0);
    } else if (fd >= 0 && fd < _getmaxstdio()) {
        return _read(fd, buf, count);
    }
    return -1;
}

long android_write(int fd, void *buf, size_t count) {
    if (is_socket(fd)) {
        return send(fd, buf, count, 0);
    } else if (fd >= 0 && fd < _getmaxstdio()) {
        return _write(fd, buf, count);
    }
    return -1;
}
#endif
