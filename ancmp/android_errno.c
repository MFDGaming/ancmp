#include "android_errno.h"
#include <errno.h>
#include "android_atomic.h"
#include <string.h>
#include <stdlib.h>

int no_to_android(int no) {
    int android_no;

    if (no == 0) {
        android_no = 0;
    } else
#ifdef EPERM
    if (no == EPERM) {
        android_no = ANDROID_EPERM;
    } else
#endif
#ifdef ENOENT
    if (no == ENOENT) {
        android_no = ANDROID_ENOENT;
    } else
#endif
#ifdef ESRCH
    if (no == ESRCH) {
        android_no = ANDROID_ESRCH;
    } else
#endif
#ifdef EINTR
    if (no == EINTR) {
        android_no = ANDROID_EINTR;
    } else
#endif
#ifdef EIO
    if (no == EIO) {
        android_no = ANDROID_EIO;
    } else
#endif
#ifdef ENXIO
    if (no == ENXIO) {
        android_no = ANDROID_ENXIO;
    } else
#endif
#ifdef E2BIG
    if (no == E2BIG) {
        android_no = ANDROID_E2BIG;
    } else
#endif
#ifdef ENOEXEC
    if (no == ENOEXEC) {
        android_no = ANDROID_ENOEXEC;
    } else
#endif
#ifdef EBADF
    if (no == EBADF) {
        android_no = ANDROID_EBADF;
    } else
#endif
#ifdef ECHILD
    if (no == ECHILD) {
        android_no = ANDROID_ECHILD;
    } else
#endif
#ifdef EAGAIN
    if (no == EAGAIN) {
        android_no = ANDROID_EAGAIN;
    } else
#endif
#ifdef EWOULDBLOCK
    if (no == EWOULDBLOCK) {
        android_no = ANDROID_EWOULDBLOCK;
    } else
#endif
#ifdef ENOMEM
    if (no == ENOMEM) {
        android_no = ANDROID_ENOMEM;
    } else
#endif
#ifdef EACCES
    if (no == EACCES) {
        android_no = ANDROID_EACCES;
    } else
#endif
#ifdef EFAULT
    if (no == EFAULT) {
        android_no = ANDROID_EFAULT;
    } else
#endif
#ifdef ENOTBLK
    if (no == ENOTBLK) {
        android_no = ANDROID_ENOTBLK;
    } else
#endif
#ifdef EBUSY
    if (no == EBUSY) {
        android_no = ANDROID_EBUSY;
    } else
#endif
#ifdef EEXIST
    if (no == EEXIST) {
        android_no = ANDROID_EEXIST;
    } else
#endif
#ifdef EXDEV
    if (no == EXDEV) {
        android_no = ANDROID_EXDEV;
    } else
#endif
#ifdef ENODEV
    if (no == ENODEV) {
        android_no = ANDROID_ENODEV;
    } else
#endif
#ifdef ENOTDIR
    if (no == ENOTDIR) {
        android_no = ANDROID_ENOTDIR;
    } else
#endif
#ifdef EISDIR
    if (no == EISDIR) {
        android_no = ANDROID_EISDIR;
    } else
#endif
#ifdef EINVAL
    if (no == EINVAL) {
        android_no = ANDROID_EINVAL;
    } else
#endif
#ifdef ENFILE
    if (no == ENFILE) {
        android_no = ANDROID_ENFILE;
    } else
#endif
#ifdef EMFILE
    if (no == EMFILE) {
        android_no = ANDROID_EMFILE;
    } else
#endif
#ifdef ENOTTY
    if (no == ENOTTY) {
        android_no = ANDROID_ENOTTY;
    } else
#endif
#ifdef ETXTBSY
    if (no == ETXTBSY) {
        android_no = ANDROID_ETXTBSY;
    } else
#endif
#ifdef EFBIG
    if (no == EFBIG) {
        android_no = ANDROID_EFBIG;
    } else
#endif
#ifdef ENOSPC
    if (no == ENOSPC) {
        android_no = ANDROID_ENOSPC;
    } else
#endif
#ifdef ESPIPE
    if (no == ESPIPE) {
        android_no = ANDROID_ESPIPE;
    } else
#endif
#ifdef EROFS
    if (no == EROFS) {
        android_no = ANDROID_EROFS;
    } else
#endif
#ifdef EMLINK
    if (no == EMLINK) {
        android_no = ANDROID_EMLINK;
    } else
#endif
#ifdef EPIPE
    if (no == EPIPE) {
        android_no = ANDROID_EPIPE;
    } else
#endif
#ifdef EDOM
    if (no == EDOM) {
        android_no = ANDROID_EDOM;
    } else
#endif
#ifdef ERANGE
    if (no == ERANGE) {
        android_no = ANDROID_ERANGE;
    } else
#endif
#ifdef EDEADLK
    if (no == EDEADLK) {
        android_no = ANDROID_EDEADLOCK;
    } else
#endif
#ifdef ENAMETOOLONG
    if (no == ENAMETOOLONG) {
        android_no = ANDROID_ENAMETOOLONG;
    } else
#endif
#ifdef ENOLCK
    if (no == ENOLCK) {
        android_no = ANDROID_ENOLCK;
    } else
#endif
#ifdef ENOSYS
    if (no == ENOSYS) {
        android_no = ANDROID_ENOSYS;
    } else
#endif
#ifdef ENOTEMPTY
    if (no == ENOTEMPTY) {
        android_no = ANDROID_ENOTEMPTY;
    } else
#endif
#ifdef ELOOP
    if (no == ELOOP) {
        android_no = ANDROID_ELOOP;
    } else
#endif
#ifdef ENOMSG
    if (no == ENOMSG) {
        android_no = ANDROID_ENOMSG;
    } else
#endif
#ifdef EIDRM
    if (no == EIDRM) {
        android_no = ANDROID_EIDRM;
    } else
#endif
#ifdef ECHRNG
    if (no == ECHRNG) {
        android_no = ANDROID_ECHRNG;
    } else
#endif
#ifdef EL2NSYNC
    if (no == EL2NSYNC) {
        android_no = ANDROID_EL2NSYNC;
    } else
#endif
#ifdef EL3HLT
    if (no == EL3HLT) {
        android_no = ANDROID_EL3HLT;
    } else
#endif
#ifdef EL3RST
    if (no == EL3RST) {
        android_no = ANDROID_EL3RST;
    } else
#endif
#ifdef ELNRNG
    if (no == ELNRNG) {
        android_no = ANDROID_ELNRNG;
    } else
#endif
#ifdef EUNATCH
    if (no == EUNATCH) {
        android_no = ANDROID_EUNATCH;
    } else
#endif
#ifdef ENOCSI
    if (no == ENOCSI) {
        android_no = ANDROID_ENOCSI;
    } else
#endif
#ifdef EL2HLT
    if (no == EL2HLT) {
        android_no = ANDROID_EL2HLT;
    } else
#endif
#ifdef EBADE
    if (no == EBADE) {
        android_no = ANDROID_EBADE;
    } else
#endif
#ifdef EBADR
    if (no == EBADR) {
        android_no = ANDROID_EBADR;
    } else
#endif
#ifdef EXFULL
    if (no == EXFULL) {
        android_no = ANDROID_EXFULL;
    } else
#endif
#ifdef ENOANO
    if (no == ENOANO) {
        android_no = ANDROID_ENOANO;
    } else
#endif
#ifdef EBADRQC
    if (no == EBADRQC) {
        android_no = ANDROID_EBADRQC;
    } else
#endif
#ifdef EBADSLT
    if (no == EBADSLT) {
        android_no = ANDROID_EBADSLT;
    } else
#endif
#ifdef EBFONT
    if (no == EBFONT) {
        android_no = ANDROID_EBFONT;
    } else
#endif
#ifdef ENOSTR
    if (no == ENOSTR) {
        android_no = ANDROID_ENOSTR;
    } else
#endif
#ifdef ENODATA
    if (no == ENODATA) {
        android_no = ANDROID_ENODATA;
    } else
#endif
#ifdef ETIME
    if (no == ETIME) {
        android_no = ANDROID_ETIME;
    } else
#endif
#ifdef ENOSR
    if (no == ENOSR) {
        android_no = ANDROID_ENOSR;
    } else
#endif
#ifdef ENONET
    if (no == ENONET) {
        android_no = ANDROID_ENONET;
    } else
#endif
#ifdef ENOPKG
    if (no == ENOPKG) {
        android_no = ANDROID_ENOPKG;
    } else
#endif
#ifdef EREMOTE
    if (no == EREMOTE) {
        android_no = ANDROID_EREMOTE;
    } else
#endif
#ifdef ENOLINK
    if (no == ENOLINK) {
        android_no = ANDROID_ENOLINK;
    } else
#endif
#ifdef EADV
    if (no == EADV) {
        android_no = ANDROID_EADV;
    } else
#endif
#ifdef ESRMNT
    if (no == ESRMNT) {
        android_no = ANDROID_ESRMNT;
    } else
#endif
#ifdef ECOMM
    if (no == ECOMM) {
        android_no = ANDROID_ECOMM;
    } else
#endif
#ifdef EPROTO
    if (no == EPROTO) {
        android_no = ANDROID_EPROTO;
    } else
#endif
#ifdef EMULTIHOP
    if (no == EMULTIHOP) {
        android_no = ANDROID_EMULTIHOP;
    } else
#endif
#ifdef EDOTDOT
    if (no == EDOTDOT) {
        android_no = ANDROID_EDOTDOT;
    } else
#endif
#ifdef EBADMSG
    if (no == EBADMSG) {
        android_no = ANDROID_EBADMSG;
    } else
#endif
#ifdef EOVERFLOW
    if (no == EOVERFLOW) {
        android_no = ANDROID_EOVERFLOW;
    } else
#endif
#ifdef ENOTUNIQ
    if (no == ENOTUNIQ) {
        android_no = ANDROID_ENOTUNIQ;
    } else
#endif
#ifdef EBADFD
    if (no == EBADFD) {
        android_no = ANDROID_EBADFD;
    } else
#endif
#ifdef EREMCHG
    if (no == EREMCHG) {
        android_no = ANDROID_EREMCHG;
    } else
#endif
#ifdef ELIBACC
    if (no == ELIBACC) {
        android_no = ANDROID_ELIBACC;
    } else
#endif
#ifdef ELIBBAD
    if (no == ELIBBAD) {
        android_no = ANDROID_ELIBBAD;
    } else
#endif
#ifdef ELIBSCN
    if (no == ELIBSCN) {
        android_no = ANDROID_ELIBSCN;
    } else
#endif
#ifdef ELIBMAX
    if (no == ELIBMAX) {
        android_no = ANDROID_ELIBMAX;
    } else
#endif
#ifdef ELIBEXEC
    if (no == ELIBEXEC) {
        android_no = ANDROID_ELIBEXEC;
    } else
#endif
#ifdef EILSEQ
    if (no == EILSEQ) {
        android_no = ANDROID_EILSEQ;
    } else
#endif
#ifdef ERESTART
    if (no == ERESTART) {
        android_no = ANDROID_ERESTART;
    } else
#endif
#ifdef ESTRPIPE
    if (no == ESTRPIPE) {
        android_no = ANDROID_ESTRPIPE;
    } else
#endif
#ifdef EUSERS
    if (no == EUSERS) {
        android_no = ANDROID_EUSERS;
    } else
#endif
#ifdef ENOTSOCK
    if (no == ENOTSOCK) {
        android_no = ANDROID_ENOTSOCK;
    } else
#endif
#ifdef EDESTADDRREQ
    if (no == EDESTADDRREQ) {
        android_no = ANDROID_EDESTADDRREQ;
    } else
#endif
#ifdef EMSGSIZE
    if (no == EMSGSIZE) {
        android_no = ANDROID_EMSGSIZE;
    } else
#endif
#ifdef EPROTOTYPE
    if (no == EPROTOTYPE) {
        android_no = ANDROID_EPROTOTYPE;
    } else
#endif
#ifdef ENOPROTOOPT
    if (no == ENOPROTOOPT) {
        android_no = ANDROID_ENOPROTOOPT;
    } else
#endif
#ifdef EPROTONOSUPPORT
    if (no == EPROTONOSUPPORT) {
        android_no = ANDROID_EPROTONOSUPPORT;
    } else
#endif
#ifdef ESOCKTNOSUPPORT
    if (no == ESOCKTNOSUPPORT) {
        android_no = ANDROID_ESOCKTNOSUPPORT;
    } else
#endif
#ifdef EOPNOTSUPP
    if (no == EOPNOTSUPP) {
        android_no = ANDROID_EOPNOTSUPP;
    } else
#endif
#ifdef ENOTSUP
    if (no == ENOTSUP) {
        android_no = ANDROID_ENOTSUP;
    } else
#endif
#ifdef EPFNOSUPPORT
    if (no == EPFNOSUPPORT) {
        android_no = ANDROID_EPFNOSUPPORT;
    } else
#endif
#ifdef EAFNOSUPPORT
    if (no == EAFNOSUPPORT) {
        android_no = ANDROID_EAFNOSUPPORT;
    } else
#endif
#ifdef EADDRINUSE
    if (no == EADDRINUSE) {
        android_no = ANDROID_EADDRINUSE;
    } else
#endif
#ifdef EADDRNOTAVAIL
    if (no == EADDRNOTAVAIL) {
        android_no = ANDROID_EADDRNOTAVAIL;
    } else
#endif
#ifdef ENETDOWN
    if (no == ENETDOWN) {
        android_no = ANDROID_ENETDOWN;
    } else
#endif
#ifdef ENETUNREACH
    if (no == ENETUNREACH) {
        android_no = ANDROID_ENETUNREACH;
    } else
#endif
#ifdef ENETRESET
    if (no == ENETRESET) {
        android_no = ANDROID_ENETRESET;
    } else
#endif
#ifdef ECONNABORTED
    if (no == ECONNABORTED) {
        android_no = ANDROID_ECONNABORTED;
    } else
#endif
#ifdef ECONNRESET
    if (no == ECONNRESET) {
        android_no = ANDROID_ECONNRESET;
    } else
#endif
#ifdef ENOBUFS
    if (no == ENOBUFS) {
        android_no = ANDROID_ENOBUFS;
    } else
#endif
#ifdef EISCONN
    if (no == EISCONN) {
        android_no = ANDROID_EISCONN;
    } else
#endif
#ifdef ENOTCONN
    if (no == ENOTCONN) {
        android_no = ANDROID_ENOTCONN;
    } else
#endif
#ifdef ESHUTDOWN
    if (no == ESHUTDOWN) {
        android_no = ANDROID_ESHUTDOWN;
    } else
#endif
#ifdef ETOOMANYREFS
    if (no == ETOOMANYREFS) {
        android_no = ANDROID_ETOOMANYREFS;
    } else
#endif
#ifdef ETIMEDOUT
    if (no == ETIMEDOUT) {
        android_no = ANDROID_ETIMEDOUT;
    } else
#endif
#ifdef ECONNREFUSED
    if (no == ECONNREFUSED) {
        android_no = ANDROID_ECONNREFUSED;
    } else
#endif
#ifdef EHOSTDOWN
    if (no == EHOSTDOWN) {
        android_no = ANDROID_EHOSTDOWN;
    } else
#endif
#ifdef EHOSTUNREACH
    if (no == EHOSTUNREACH) {
        android_no = ANDROID_EHOSTUNREACH;
    } else
#endif
#ifdef EALREADY
    if (no == EALREADY) {
        android_no = ANDROID_EALREADY;
    } else
#endif
#ifdef EINPROGRESS
    if (no == EINPROGRESS) {
        android_no = ANDROID_EINPROGRESS;
    } else
#endif
#ifdef ESTALE
    if (no == ESTALE) {
        android_no = ANDROID_ESTALE;
    } else
#endif
#ifdef EUCLEAN
    if (no == EUCLEAN) {
        android_no = ANDROID_EUCLEAN;
    } else
#endif
#ifdef ENOTNAM
    if (no == ENOTNAM) {
        android_no = ANDROID_ENOTNAM;
    } else
#endif
#ifdef ENAVAIL
    if (no == ENAVAIL) {
        android_no = ANDROID_ENAVAIL;
    } else
#endif
#ifdef EISNAM
    if (no == EISNAM) {
        android_no = ANDROID_EISNAM;
    } else
#endif
#ifdef EREMOTEIO
    if (no == EREMOTEIO) {
        android_no = ANDROID_EREMOTEIO;
    } else
#endif
#ifdef EDQUOT
    if (no == EDQUOT) {
        android_no = ANDROID_EDQUOT;
    } else
#endif
#ifdef ENOMEDIUM
    if (no == ENOMEDIUM) {
        android_no = ANDROID_ENOMEDIUM;
    } else
#endif
#ifdef EMEDIUMTYPE
    if (no == EMEDIUMTYPE) {
        android_no = ANDROID_EMEDIUMTYPE;
    } else
#endif
#ifdef ECANCELED
    if (no == ECANCELED) {
        android_no = ANDROID_ECANCELED;
    } else
#endif
#ifdef ENOKEY
    if (no == ENOKEY) {
        android_no = ANDROID_ENOKEY;
    } else
#endif
#ifdef EKEYEXPIRED
    if (no == EKEYEXPIRED) {
        android_no = ANDROID_EKEYEXPIRED;
    } else
#endif
#ifdef EKEYREVOKED
    if (no == EKEYREVOKED) {
        android_no = ANDROID_EKEYREVOKED;
    } else
#endif
#ifdef EKEYREJECTED
    if (no == EKEYREJECTED) {
        android_no = ANDROID_EKEYREJECTED;
    } else
#endif
#ifdef EOWNERDEAD
    if (no == EOWNERDEAD) {
        android_no = ANDROID_EOWNERDEAD;
    } else
#endif
#ifdef ENOTRECOVERABLE
    if (no == ENOTRECOVERABLE) {
        android_no = ANDROID_ENOTRECOVERABLE;
    } else
#endif
    {
        android_no = no;
    }
    return android_no;
}

int no_to_native(int android_no) {
    int no;

    if (android_no == 0) {
        no = 0;
    } else
#ifdef EPERM
    if (android_no == ANDROID_EPERM) {
        no = EPERM;
    } else
#endif
#ifdef ENOENT
    if (android_no == ANDROID_ENOENT) {
        no = ENOENT;
    } else
#endif
#ifdef ESRCH
    if (android_no == ANDROID_ESRCH) {
        no = ESRCH;
    } else
#endif
#ifdef EINTR
    if (android_no == ANDROID_EINTR) {
        no = EINTR;
    } else
#endif
#ifdef EIO
    if (android_no == ANDROID_EIO) {
        no = EIO;
    } else
#endif
#ifdef ENXIO
    if (android_no == ANDROID_ENXIO) {
        no = ENXIO;
    } else
#endif
#ifdef E2BIG
    if (android_no == ANDROID_E2BIG) {
        no = E2BIG;
    } else
#endif
#ifdef ENOEXEC
    if (android_no == ANDROID_ENOEXEC) {
        no = ENOEXEC;
    } else
#endif
#ifdef EBADF
    if (android_no == ANDROID_EBADF) {
        no = EBADF;
    } else
#endif
#ifdef ECHILD
    if (android_no == ANDROID_ECHILD) {
        no = ECHILD;
    } else
#endif
#ifdef EAGAIN
    if (android_no == ANDROID_EAGAIN) {
        no = EAGAIN;
    } else
#endif
#ifdef EWOULDBLOCK
    if (android_no == ANDROID_EWOULDBLOCK) {
        no = EWOULDBLOCK;
    } else
#endif
#ifdef ENOMEM
    if (android_no == ANDROID_ENOMEM) {
        no = ENOMEM;
    } else
#endif
#ifdef EACCES
    if (android_no == ANDROID_EACCES) {
        no = EACCES;
    } else
#endif
#ifdef EFAULT
    if (android_no == ANDROID_EFAULT) {
        no = EFAULT;
    } else
#endif
#ifdef ENOTBLK
    if (android_no == ANDROID_ENOTBLK) {
        no = ENOTBLK;
    } else
#endif
#ifdef EBUSY
    if (android_no == ANDROID_EBUSY) {
        no = EBUSY;
    } else
#endif
#ifdef EEXIST
    if (android_no == ANDROID_EEXIST) {
        no = EEXIST;
    } else
#endif
#ifdef EXDEV
    if (android_no == ANDROID_EXDEV) {
        no = EXDEV;
    } else
#endif
#ifdef ENODEV
    if (android_no == ANDROID_ENODEV) {
        no = ENODEV;
    } else
#endif
#ifdef ENOTDIR
    if (android_no == ANDROID_ENOTDIR) {
        no = ENOTDIR;
    } else
#endif
#ifdef EISDIR
    if (android_no == ANDROID_EISDIR) {
        no = EISDIR;
    } else
#endif
#ifdef EINVAL
    if (android_no == ANDROID_EINVAL) {
        no = EINVAL;
    } else
#endif
#ifdef ENFILE
    if (android_no == ANDROID_ENFILE) {
        no = ENFILE;
    } else
#endif
#ifdef EMFILE
    if (android_no == ANDROID_EMFILE) {
        no = EMFILE;
    } else
#endif
#ifdef ENOTTY
    if (android_no == ANDROID_ENOTTY) {
        no = ENOTTY;
    } else
#endif
#ifdef ETXTBSY
    if (android_no == ANDROID_ETXTBSY) {
        no = ETXTBSY;
    } else
#endif
#ifdef EFBIG
    if (android_no == ANDROID_EFBIG) {
        no = EFBIG;
    } else
#endif
#ifdef ENOSPC
    if (android_no == ANDROID_ENOSPC) {
        no = ENOSPC;
    } else
#endif
#ifdef ESPIPE
    if (android_no == ANDROID_ESPIPE) {
        no = ESPIPE;
    } else
#endif
#ifdef EROFS
    if (android_no == ANDROID_EROFS) {
        no = EROFS;
    } else
#endif
#ifdef EMLINK
    if (android_no == ANDROID_EMLINK) {
        no = EMLINK;
    } else
#endif
#ifdef EPIPE
    if (android_no == ANDROID_EPIPE) {
        no = EPIPE;
    } else
#endif
#ifdef EDOM
    if (android_no == ANDROID_EDOM) {
        no = EDOM;
    } else
#endif
#ifdef ERANGE
    if (android_no == ANDROID_ERANGE) {
        no = ERANGE;
    } else
#endif
#ifdef EDEADLK
    if (android_no == ANDROID_EDEADLK) {
        no = EDEADLOCK;
    } else
#endif
#ifdef ENAMETOOLONG
    if (android_no == ANDROID_ENAMETOOLONG) {
        no = ENAMETOOLONG;
    } else
#endif
#ifdef ENOLCK
    if (android_no == ANDROID_ENOLCK) {
        no = ENOLCK;
    } else
#endif
#ifdef ENOSYS
    if (android_no == ANDROID_ENOSYS) {
        no = ENOSYS;
    } else
#endif
#ifdef ENOTEMPTY
    if (android_no == ANDROID_ENOTEMPTY) {
        no = ENOTEMPTY;
    } else
#endif
#ifdef ELOOP
    if (android_no == ANDROID_ELOOP) {
        no = ELOOP;
    } else
#endif
#ifdef ENOMSG
    if (android_no == ANDROID_ENOMSG) {
        no = ENOMSG;
    } else
#endif
#ifdef EIDRM
    if (android_no == ANDROID_EIDRM) {
        no = EIDRM;
    } else
#endif
#ifdef ECHRNG
    if (android_no == ANDROID_ECHRNG) {
        no = ECHRNG;
    } else
#endif
#ifdef EL2NSYNC
    if (android_no == ANDROID_EL2NSYNC) {
        no = EL2NSYNC;
    } else
#endif
#ifdef EL3HLT
    if (android_no == ANDROID_EL3HLT) {
        no = EL3HLT;
    } else
#endif
#ifdef EL3RST
    if (android_no == ANDROID_EL3RST) {
        no = EL3RST;
    } else
#endif
#ifdef ELNRNG
    if (android_no == ANDROID_ELNRNG) {
        no = ELNRNG;
    } else
#endif
#ifdef EUNATCH
    if (android_no == ANDROID_EUNATCH) {
        no = EUNATCH;
    } else
#endif
#ifdef ENOCSI
    if (android_no == ANDROID_ENOCSI) {
        no = ENOCSI;
    } else
#endif
#ifdef EL2HLT
    if (android_no == ANDROID_EL2HLT) {
        no = EL2HLT;
    } else
#endif
#ifdef EBADE
    if (android_no == ANDROID_EBADE) {
        no = EBADE;
    } else
#endif
#ifdef EBADR
    if (android_no == ANDROID_EBADR) {
        no = EBADR;
    } else
#endif
#ifdef EXFULL
    if (android_no == ANDROID_EXFULL) {
        no = EXFULL;
    } else
#endif
#ifdef ENOANO
    if (android_no == ANDROID_ENOANO) {
        no = ENOANO;
    } else
#endif
#ifdef EBADRQC
    if (android_no == ANDROID_EBADRQC) {
        no = EBADRQC;
    } else
#endif
#ifdef EBADSLT
    if (android_no == ANDROID_EBADSLT) {
        no = EBADSLT;
    } else
#endif
#ifdef EBFONT
    if (android_no == ANDROID_EBFONT) {
        no = EBFONT;
    } else
#endif
#ifdef ENOSTR
    if (android_no == ANDROID_ENOSTR) {
        no = ENOSTR;
    } else
#endif
#ifdef ENODATA
    if (android_no == ANDROID_ENODATA) {
        no = ENODATA;
    } else
#endif
#ifdef ETIME
    if (android_no == ANDROID_ETIME) {
        no = ETIME;
    } else
#endif
#ifdef ENOSR
    if (android_no == ANDROID_ENOSR) {
        no = ENOSR;
    } else
#endif
#ifdef ENONET
    if (android_no == ANDROID_ENONET) {
        no = ENONET;
    } else
#endif
#ifdef ENOPKG
    if (android_no == ANDROID_ENOPKG) {
        no = ENOPKG;
    } else
#endif
#ifdef EREMOTE
    if (android_no == ANDROID_EREMOTE) {
        no = EREMOTE;
    } else
#endif
#ifdef ENOLINK
    if (android_no == ANDROID_ENOLINK) {
        no = ENOLINK;
    } else
#endif
#ifdef EADV
    if (android_no == ANDROID_EADV) {
        no = EADV;
    } else
#endif
#ifdef ESRMNT
    if (android_no == ANDROID_ESRMNT) {
        no = ESRMNT;
    } else
#endif
#ifdef ECOMM
    if (android_no == ANDROID_ECOMM) {
        no = ECOMM;
    } else
#endif
#ifdef EPROTO
    if (android_no == ANDROID_EPROTO) {
        no = EPROTO;
    } else
#endif
#ifdef EMULTIHOP
    if (android_no == ANDROID_EMULTIHOP) {
        no = EMULTIHOP;
    } else
#endif
#ifdef EDOTDOT
    if (android_no == ANDROID_EDOTDOT) {
        no = EDOTDOT;
    } else
#endif
#ifdef EBADMSG
    if (android_no == ANDROID_EBADMSG) {
        no = EBADMSG;
    } else
#endif
#ifdef EOVERFLOW
    if (android_no == ANDROID_EOVERFLOW) {
        no = EOVERFLOW;
    } else
#endif
#ifdef ENOTUNIQ
    if (android_no == ANDROID_ENOTUNIQ) {
        no = ENOTUNIQ;
    } else
#endif
#ifdef EBADFD
    if (android_no == ANDROID_EBADFD) {
        no = EBADFD;
    } else
#endif
#ifdef EREMCHG
    if (android_no == ANDROID_EREMCHG) {
        no = EREMCHG;
    } else
#endif
#ifdef ELIBACC
    if (android_no == ANDROID_ELIBACC) {
        no = ELIBACC;
    } else
#endif
#ifdef ELIBBAD
    if (android_no == ANDROID_ELIBBAD) {
        no = ELIBBAD;
    } else
#endif
#ifdef ELIBSCN
    if (android_no == ANDROID_ELIBSCN) {
        no = ELIBSCN;
    } else
#endif
#ifdef ELIBMAX
    if (android_no == ANDROID_ELIBMAX) {
        no = ELIBMAX;
    } else
#endif
#ifdef ELIBEXEC
    if (android_no == ANDROID_ELIBEXEC) {
        no = ELIBEXEC;
    } else
#endif
#ifdef EILSEQ
    if (android_no == ANDROID_EILSEQ) {
        no = EILSEQ;
    } else
#endif
#ifdef ERESTART
    if (android_no == ANDROID_ERESTART) {
        no = ERESTART;
    } else
#endif
#ifdef ESTRPIPE
    if (android_no == ANDROID_ESTRPIPE) {
        no = ESTRPIPE;
    } else
#endif
#ifdef EUSERS
    if (android_no == ANDROID_EUSERS) {
        no = EUSERS;
    } else
#endif
#ifdef ENOTSOCK
    if (android_no == ANDROID_ENOTSOCK) {
        no = ENOTSOCK;
    } else
#endif
#ifdef EDESTADDRREQ
    if (android_no == ANDROID_EDESTADDRREQ) {
        no = EDESTADDRREQ;
    } else
#endif
#ifdef EMSGSIZE
    if (android_no == ANDROID_EMSGSIZE) {
        no = EMSGSIZE;
    } else
#endif
#ifdef EPROTOTYPE
    if (android_no == ANDROID_EPROTOTYPE) {
        no = EPROTOTYPE;
    } else
#endif
#ifdef ENOPROTOOPT
    if (android_no == ANDROID_ENOPROTOOPT) {
        no = ENOPROTOOPT;
    } else
#endif
#ifdef EPROTONOSUPPORT
    if (android_no == ANDROID_EPROTONOSUPPORT) {
        no = EPROTONOSUPPORT;
    } else
#endif
#ifdef ESOCKTNOSUPPORT
    if (android_no == ANDROID_ESOCKTNOSUPPORT) {
        no = ESOCKTNOSUPPORT;
    } else
#endif
#ifdef EOPNOTSUPP
    if (android_no == ANDROID_EOPNOTSUPP) {
        no = EOPNOTSUPP;
    } else
#endif
#ifdef ENOTSUP
    if (android_no == ANDROID_ENOTSUP) {
        no = ENOTSUP;
    } else
#endif
#ifdef EPFNOSUPPORT
    if (android_no == ANDROID_EPFNOSUPPORT) {
        no = EPFNOSUPPORT;
    } else
#endif
#ifdef EAFNOSUPPORT
    if (android_no == ANDROID_EAFNOSUPPORT) {
        no = EAFNOSUPPORT;
    } else
#endif
#ifdef EADDRINUSE
    if (android_no == ANDROID_EADDRINUSE) {
        no = EADDRINUSE;
    } else
#endif
#ifdef EADDRNOTAVAIL
    if (android_no == ANDROID_EADDRNOTAVAIL) {
        no = EADDRNOTAVAIL;
    } else
#endif
#ifdef ENETDOWN
    if (android_no == ANDROID_ENETDOWN) {
        no = ENETDOWN;
    } else
#endif
#ifdef ENETUNREACH
    if (android_no == ANDROID_ENETUNREACH) {
        no = ENETUNREACH;
    } else
#endif
#ifdef ENETRESET
    if (android_no == ANDROID_ENETRESET) {
        no = ENETRESET;
    } else
#endif
#ifdef ECONNABORTED
    if (android_no == ANDROID_ECONNABORTED) {
        no = ECONNABORTED;
    } else
#endif
#ifdef ECONNRESET
    if (android_no == ANDROID_ECONNRESET) {
        no = ECONNRESET;
    } else
#endif
#ifdef ENOBUFS
    if (android_no == ANDROID_ENOBUFS) {
        no = ENOBUFS;
    } else
#endif
#ifdef EISCONN
    if (android_no == ANDROID_EISCONN) {
        no = EISCONN;
    } else
#endif
#ifdef ENOTCONN
    if (android_no == ANDROID_ENOTCONN) {
        no = ENOTCONN;
    } else
#endif
#ifdef ESHUTDOWN
    if (android_no == ANDROID_ESHUTDOWN) {
        no = ESHUTDOWN;
    } else
#endif
#ifdef ETOOMANYREFS
    if (android_no == ANDROID_ETOOMANYREFS) {
        no = ETOOMANYREFS;
    } else
#endif
#ifdef ETIMEDOUT
    if (android_no == ANDROID_ETIMEDOUT) {
        no = ETIMEDOUT;
    } else
#endif
#ifdef ECONNREFUSED
    if (android_no == ANDROID_ECONNREFUSED) {
        no = ECONNREFUSED;
    } else
#endif
#ifdef EHOSTDOWN
    if (android_no == ANDROID_EHOSTDOWN) {
        no = EHOSTDOWN;
    } else
#endif
#ifdef EHOSTUNREACH
    if (android_no == ANDROID_EHOSTUNREACH) {
        no = EHOSTUNREACH;
    } else
#endif
#ifdef EALREADY
    if (android_no == ANDROID_EALREADY) {
        no = EALREADY;
    } else
#endif
#ifdef EINPROGRESS
    if (android_no == ANDROID_EINPROGRESS) {
        no = EINPROGRESS;
    } else
#endif
#ifdef ESTALE
    if (android_no == ANDROID_ESTALE) {
        no = ESTALE;
    } else
#endif
#ifdef EUCLEAN
    if (android_no == ANDROID_EUCLEAN) {
        no = EUCLEAN;
    } else
#endif
#ifdef ENOTNAM
    if (android_no == ANDROID_ENOTNAM) {
        no = ENOTNAM;
    } else
#endif
#ifdef ENAVAIL
    if (android_no == ANDROID_ENAVAIL) {
        no = ENAVAIL;
    } else
#endif
#ifdef EISNAM
    if (android_no == ANDROID_EISNAM) {
        no = EISNAM;
    } else
#endif
#ifdef EREMOTEIO
    if (android_no == ANDROID_EREMOTEIO) {
        no = EREMOTEIO;
    } else
#endif
#ifdef EDQUOT
    if (android_no == ANDROID_EDQUOT) {
        no = EDQUOT;
    } else
#endif
#ifdef ENOMEDIUM
    if (android_no == ANDROID_ENOMEDIUM) {
        no = ENOMEDIUM;
    } else
#endif
#ifdef EMEDIUMTYPE
    if (android_no == ANDROID_EMEDIUMTYPE) {
        no = EMEDIUMTYPE;
    } else
#endif
#ifdef ECANCELED
    if (android_no == ANDROID_ECANCELED) {
        no = ECANCELED;
    } else
#endif
#ifdef ENOKEY
    if (android_no == ANDROID_ENOKEY) {
        no = ENOKEY;
    } else
#endif
#ifdef EKEYEXPIRED
    if (android_no == ANDROID_EKEYEXPIRED) {
        no = EKEYEXPIRED;
    } else
#endif
#ifdef EKEYREVOKED
    if (android_no == ANDROID_EKEYREVOKED) {
        no = EKEYREVOKED;
    } else
#endif
#ifdef EKEYREJECTED
    if (android_no == ANDROID_EKEYREJECTED) {
        no = EKEYREJECTED;
    } else
#endif
#ifdef EOWNERDEAD
    if (android_no == ANDROID_EOWNERDEAD) {
        no = EOWNERDEAD;
    } else
#endif
#ifdef ENOTRECOVERABLE
    if (android_no == ANDROID_ENOTRECOVERABLE) {
        no = ENOTRECOVERABLE;
    } else
#endif
    {
        no = android_no;
    }
    return no;
}

static const char *errnum_to_str(int errnum) {
    switch (errnum) {
        case ANDROID_EPERM: return "Operation not permitted";
        case ANDROID_ENOENT: return "No such file or directory";
        case ANDROID_ESRCH: return "No such process";
        case ANDROID_EINTR: return "Interrupted system call";
        case ANDROID_EIO: return "I/O error";
        case ANDROID_ENXIO: return "No such device or address";
        case ANDROID_E2BIG: return "Argument list too long";
        case ANDROID_ENOEXEC: return "Exec format error";
        case ANDROID_EBADF: return "Bad file descriptor";
        case ANDROID_ECHILD: return "No child processes";
        case ANDROID_EAGAIN: return "Resource temporarily unavailable";
        case ANDROID_ENOMEM: return "Out of memory";
        case ANDROID_EACCES: return "Permission denied";
        case ANDROID_EFAULT: return "Bad address";
        case ANDROID_ENOTBLK: return "Block device required";
        case ANDROID_EBUSY: return "Device or resource busy";
        case ANDROID_EEXIST: return "File exists";
        case ANDROID_EXDEV: return "Invalid cross-device link";
        case ANDROID_ENODEV: return "No such device";
        case ANDROID_ENOTDIR: return "Not a directory";
        case ANDROID_EISDIR: return "Is a directory";
        case ANDROID_EINVAL: return "Invalid argument";
        case ANDROID_ENFILE: return "File table overflow";
        case ANDROID_EMFILE: return "Too many open files";
        case ANDROID_ENOTTY: return "Inappropriate I/O control operation";
        case ANDROID_ETXTBSY: return "Text file busy";
        case ANDROID_EFBIG: return "File too large";
        case ANDROID_ENOSPC: return "No space left on device";
        case ANDROID_ESPIPE: return "Illegal seek";
        case ANDROID_EROFS: return "Read-only file system";
        case ANDROID_EMLINK: return "Too many links";
        case ANDROID_EPIPE: return "Broken pipe";
        case ANDROID_EDOM: return "Numerical argument out of domain";
        case ANDROID_ERANGE: return "Numerical result out of range";
        case ANDROID_EDEADLK: return "Resource deadlock avoided";
        case ANDROID_ENAMETOOLONG: return "File name too long";
        case ANDROID_ENOLCK: return "No locks available";
        case ANDROID_ENOSYS: return "Function not implemented";
        case ANDROID_ENOTEMPTY: return "Directory not empty";
        case ANDROID_ELOOP: return "Too many symbolic links encountered";
        case ANDROID_ENOMSG: return "No message of desired type";
        case ANDROID_EIDRM: return "Identifier removed";
        case ANDROID_ECHRNG: return "Channel number out of range";
        case ANDROID_EL2NSYNC: return "Level 2 not synchronized";
        case ANDROID_EL3HLT: return "Level 3 halted";
        case ANDROID_EL3RST: return "Level 3 reset";
        case ANDROID_ELNRNG: return "Link number out of range";
        case ANDROID_EUNATCH: return "Protocol driver not attached";
        case ANDROID_ENOCSI: return "No CSI structure available";
        case ANDROID_EL2HLT: return "Level 2 halted";
        case ANDROID_EBADE: return "Invalid exchange";
        case ANDROID_EBADR: return "Bad request descriptor";
        case ANDROID_EXFULL: return "Exchange full";
        case ANDROID_ENOANO: return "No anode";
        case ANDROID_EBADRQC: return "Bad request code";
        case ANDROID_EBADSLT: return "Bad slot";
        case ANDROID_EBFONT: return "Bad font file format";
        case ANDROID_ENOSTR: return "Device not a stream";
        case ANDROID_ENODATA: return "No data available";
        case ANDROID_ETIME: return "Timer expired";
        case ANDROID_ENOSR: return "Out of streams resources";
        case ANDROID_ENONET: return "Machine is not on the network";
        case ANDROID_ENOPKG: return "Package not installed";
        case ANDROID_EREMOTE: return "Object is remote";
        case ANDROID_ENOLINK: return "Link has been severed";
        case ANDROID_EADV: return "Advertise error";
        case ANDROID_ESRMNT: return "Srmount error";
        case ANDROID_ECOMM: return "Communication error on send";
        case ANDROID_EPROTO: return "Protocol error";
        case ANDROID_EMULTIHOP: return "Multihop attempted";
        case ANDROID_EDOTDOT: return "RFS specific error";
        case ANDROID_EBADMSG: return "Bad message";
        case ANDROID_EOVERFLOW: return "Value too large for defined data type";
        case ANDROID_ENOTUNIQ: return "Name not unique on network";
        case ANDROID_EBADFD: return "File descriptor in bad state";
        case ANDROID_EREMCHG: return "Remote address changed";
        case ANDROID_ELIBACC: return "Can not access a needed shared library";
        case ANDROID_ELIBBAD: return "Accessing a corrupted shared library";
        case ANDROID_ELIBSCN: return "Lib section in a.out corrupted";
        case ANDROID_ELIBMAX: return "Attempting to link in too many shared libraries";
        case ANDROID_ELIBEXEC: return "Cannot exec a shared library directly";
        case ANDROID_EILSEQ: return "Illegal byte sequence";
        case ANDROID_ERESTART: return "Interrupted system call should be restarted";
        case ANDROID_ESTRPIPE: return "Streams pipe error";
        case ANDROID_EUSERS: return "Too many users";
        case ANDROID_ENOTSOCK: return "Not a socket";
        case ANDROID_EDESTADDRREQ: return "Destination address required";
        case ANDROID_EMSGSIZE: return "Message too long";
        case ANDROID_EPROTOTYPE: return "Protocol wrong type for socket";
        case ANDROID_ENOPROTOOPT: return "Protocol not available";
        case ANDROID_EPROTONOSUPPORT: return "Protocol not supported";
        case ANDROID_ESOCKTNOSUPPORT: return "Socket type not supported";
        case ANDROID_EOPNOTSUPP: return "Operation not supported on transport endpoint";
        case ANDROID_EPFNOSUPPORT: return "Protocol family not supported";
        case ANDROID_EAFNOSUPPORT: return "Address family not supported by protocol";
        case ANDROID_EADDRINUSE: return "Address already in use";
        case ANDROID_EADDRNOTAVAIL: return "Cannot assign requested address";
        case ANDROID_ENETDOWN: return "Network is down";
        case ANDROID_ENETUNREACH: return "Network is unreachable";
        case ANDROID_ENETRESET: return "Network dropped connection because of reset";
        case ANDROID_ECONNABORTED: return "Software caused connection abort";
        case ANDROID_ECONNRESET: return "Connection reset by peer";
        case ANDROID_ENOBUFS: return "No buffer space available";
        case ANDROID_EISCONN: return "Transport endpoint is already connected";
        case ANDROID_ENOTCONN: return "Transport endpoint is not connected";
        case ANDROID_ESHUTDOWN: return "Cannot send after transport endpoint shutdown";
        case ANDROID_ETOOMANYREFS: return "Too many references: cannot splice";
        case ANDROID_ETIMEDOUT: return "Operation timed out";
        case ANDROID_ECONNREFUSED: return "Connection refused";
        case ANDROID_EHOSTDOWN: return "Host is down";
        case ANDROID_EHOSTUNREACH: return "No route to host";
        case ANDROID_EALREADY: return "Operation already in progress";
        case ANDROID_EINPROGRESS: return "Operation now in progress";
        case ANDROID_ESTALE: return "Stale file handle";
        case ANDROID_EUCLEAN: return "Structure needs cleaning";
        case ANDROID_ENOTNAM: return "Not a XENIX named type file";
        case ANDROID_ENAVAIL: return "No XENIX semaphores available";
        case ANDROID_EISNAM: return "Is a named type file";
        case ANDROID_EREMOTEIO: return "Remote I/O error";
        case ANDROID_EDQUOT: return "Disk quota exceeded";
        case ANDROID_ENOMEDIUM: return "No medium found";
        case ANDROID_EMEDIUMTYPE: return "Wrong medium type";
        case ANDROID_ECANCELED: return "Operation canceled";
        case ANDROID_ENOKEY: return "Required key not available";
        case ANDROID_EKEYEXPIRED: return "Key has expired";
        case ANDROID_EKEYREVOKED: return "Key has been revoked";
        case ANDROID_EKEYREJECTED: return "Key was rejected by service";
        case ANDROID_EOWNERDEAD: return "Owner died";
        case ANDROID_ENOTRECOVERABLE: return "State not recoverable";
        default: return "Unknown error";
    }
}

#ifdef _WIN32
DWORD android_errno_key;
#else
pthread_key_t android_errno_key;
#endif

int android_errno_init(void) {
    void *errno_alloc = calloc(1, sizeof(int));
    if (errno_alloc == NULL) {
        return 0;
    }
#ifdef _WIN32
    android_errno_key = TlsAlloc();
    if (android_errno_key == TLS_OUT_OF_INDEXES) {
        free(errno_alloc);
        return 0;
    }
    if (!TlsSetValue(android_errno_key, errno_alloc)) {
        free(errno_alloc);
        return 0;
    }
#else
    if (pthread_key_create(&android_errno_key, NULL) != 0) {
        free(errno_alloc);
        return 0;
    }
    if (pthread_setspecific(android_errno_key, errno_alloc) != 0) {
        free(errno_alloc);
        return 0;
    }
#endif
    return 1;
}

int *android_errno(void) {
    static int dummy = 0;
#ifdef _WIN32
    int *ret = (int *)TlsGetValue(android_errno_key);
#else
    int *ret = (int *)pthread_getspecific(android_errno_key);
#endif
    if (!ret) {
        return &dummy;
    }
    if (errno != 0) {
        *ret = no_to_android(errno);
    }
    return ret;
}

int android_strerror_r(int errnum, char *strerrbuf, size_t buflen) {
    const char *err_str;
    size_t err_str_len, copy_size;

    if (buflen < 2) {
        return ANDROID_ERANGE;
    }
    --buflen;
    err_str = errnum_to_str(errnum);
    err_str_len = strlen(err_str);
    copy_size = (buflen > err_str_len) ? err_str_len : buflen;
    memcpy(strerrbuf, err_str, copy_size);
    strerrbuf[copy_size] = '\0';
    return (copy_size == err_str_len) ? 0 : ANDROID_ERANGE;
}

char *android_strerror(int errnum) {
    static char buf[256];
	(void)android_strerror_r(errnum, buf, sizeof(buf));
	return (buf);
}
