/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#ifndef _WIN32
#include <sys/mman.h>
#endif
#include <errno.h>
#include <stddef.h>

#include "linker.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/un.h>
#endif

void notify_gdb_of_libraries();

#define  RETRY_ON_EINTR(ret,cond) \
    do { \
        ret = (cond); \
    } while (ret < 0 && errno == EINTR)

#if 0
static int socket_abstract_client(const char *name, int type)
{
    struct sockaddr_un addr;
    size_t namelen;
    socklen_t alen;
    int s, err;

    namelen  = strlen(name);

    // Test with length +1 for the *initial* '\0'.
    if ((namelen + 1) > sizeof(addr.sun_path)) {
        errno = EINVAL;
        return -1;
    }

    /* This is used for abstract socket namespace, we need
     * an initial '\0' at the start of the Unix socket path.
     *
     * Note: The path in this case is *not* supposed to be
     * '\0'-terminated. ("man 7 unix" for the gory details.)
     */
    memset (&addr, 0, sizeof addr);
    addr.sun_family = AF_LOCAL;
    addr.sun_path[0] = 0;
    memcpy(addr.sun_path + 1, name, namelen);

    alen = namelen + offsetof(struct sockaddr_un, sun_path) + 1;

    s = socket(AF_LOCAL, type, 0);
    if(s < 0) return -1;

    RETRY_ON_EINTR(err,connect(s, (struct sockaddr *) &addr, alen));
    if (err < 0) {
        close(s);
        s = -1;
    }

    return s;
}
#endif

void debugger_signal_handler(int n)
{
#if 0
    unsigned tid;
    int s;

    /* avoid picking up GC interrupts */
    signal(SIGUSR1, SIG_IGN);

    notify_gdb_of_libraries();

    tid = gettid();
    s = socket_abstract_client("android:debuggerd", SOCK_STREAM);

    if(s >= 0) {
        /* debugger knows our pid from the credentials on the
         * local socket but we need to tell it our tid.  It
         * is paranoid and will verify that we are giving a tid
         * that's actually in our process
         */
        int  ret;

        RETRY_ON_EINTR(ret, write(s, &tid, sizeof(unsigned)));
        if (ret == sizeof(unsigned)) {
            /* if the write failed, there is no point to read on
             * the file descriptor. */
            RETRY_ON_EINTR(ret, read(s, &tid, 1));
            notify_gdb_of_libraries();
        }
        close(s);
    }

    /* remove our net so we fault for real when we return */
    signal(n, SIG_IGN);
#endif
}

void debugger_init()
{
    signal(SIGILL, debugger_signal_handler);
    signal(SIGABRT, debugger_signal_handler);
    signal(SIGFPE, debugger_signal_handler);
    signal(SIGSEGV, debugger_signal_handler);
#ifndef _WIN32
    signal(SIGBUS, debugger_signal_handler);
    signal(SIGSTKFLT, debugger_signal_handler);
    signal(SIGPIPE, debugger_signal_handler);
#endif
}