/*      $OpenBSD: arc4random.c,v 1.19 2008/06/04 00:50:23 djm Exp $        */
/*
 * Copyright (c) 1996, David Mazieres <dm@uun.org>
 * Copyright (c) 2008, Damien Miller <djm@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*
 * Arc4 random number generator for OpenBSD.
 *
 * This code is derived from section 17.1 of Applied Cryptography,
 * second edition, which describes a stream cipher allegedly
 * compatible with RSA Labs "RC4" cipher (the actual description of
 * which is a trade secret).  The same algorithm is used as a stream
 * cipher called "arcfour" in Tatu Ylonen's ssh package.
 *
 * Here the stream cipher has been modified always to include the time
 * when initializing the state.  That makes it impossible to
 * regenerate the same random sequence twice, so this can't be used
 * for encryption, but will generate good random numbers.
 *
 * RC4 is a registered trademark of RSA Laboratories.
 */

#include "android_arc4random.h"
#include "android_pthread.h"
#include "android_time.h"
#ifdef _WIN32
#include <windows.h>
#define _ARC4_PID GetCurrentProcessId()
#else
#include <unistd.h>
#define _ARC4_PID getpid()
#endif

static android_pthread_mutex_t _arc4_lock = ANDROID_PTHREAD_MUTEX_INITIALIZER;
#define _ARC4_LOCK() android_pthread_mutex_lock(&_arc4_lock)
#define _ARC4_UNLOCK() android_pthread_mutex_unlock(&_arc4_lock)

struct arc4_stream {
    uint8_t i;
    uint8_t j;
    uint8_t s[256];
};

static int rs_initialized;
static struct arc4_stream rs;
static int arc4_stir_pid;
static int arc4_count;

static uint8_t arc4_getbyte(void) {
    uint8_t si, sj;
    rs.i = (rs.i + 1);
    si = rs.s[rs.i];
    rs.j = (rs.j + si);
    sj = rs.s[rs.j];
    rs.s[rs.i] = sj;
    rs.s[rs.j] = si;
    return (rs.s[(si + sj) & 0xff]);
}

static uint32_t arc4_getword(void) {
    uint32_t val;
    val = arc4_getbyte() << 24;
    val |= arc4_getbyte() << 16;
    val |= arc4_getbyte() << 8;
    val |= arc4_getbyte();
    return val;
}

static void arc4_addrandom(unsigned char *dat, int datlen) {
    int n;
    uint8_t si;
    --rs.i;
    for (n = 0; n < 256; n++) {
        rs.i = (rs.i + 1);
        si = rs.s[rs.i];
        rs.j = (rs.j + si + dat[n % datlen]);
        rs.s[rs.i] = rs.s[rs.j];
        rs.s[rs.j] = si;
    }
    rs.j = rs.i;
}

static void arc4_init(void) {
    int n;
    for (n = 0; n < 256; n++) {
        rs.s[n] = n;
    }
    rs.i = 0;
    rs.j = 0;
}

static void arc4_stir(void) {
	int i;
	union {
		android_timeval_t tv;
		unsigned int rnd[128 / sizeof(unsigned int)];
	} rdat;

    if (!rs_initialized) {
        arc4_init();
            rs_initialized = 1;
    }
    android_gettimeofday(&rdat.tv, NULL);
    arc4_stir_pid = _ARC4_PID;
	arc4_addrandom((void *)&rdat, sizeof(rdat));
    for (i = 0; i < 256; i++) {
        (void)arc4_getbyte();
    }    
    arc4_count = 1600000;
}

uint8_t android_arc4_getbyte(void) {
    uint8_t val;
    _ARC4_LOCK();
    if (--arc4_count == 0 || !rs_initialized) {
        arc4_stir();
    }
    val = arc4_getbyte();
    _ARC4_UNLOCK();
    return val;
}

void android_arc4random_stir(void) {
    _ARC4_LOCK();
    arc4_stir();
    _ARC4_UNLOCK();
}

void android_arc4random_addrandom(unsigned char *dat, int datlen) {
    _ARC4_LOCK();
    if (!rs_initialized) {
        arc4_stir();
    }
    arc4_addrandom(dat, datlen);
    _ARC4_UNLOCK();
}

uint32_t android_arc4random(void) {
    uint32_t val;
    _ARC4_LOCK();
    arc4_count -= 4;
    if (arc4_count <= 0 || !rs_initialized || arc4_stir_pid != _ARC4_PID) {
        arc4_stir();
    }
    val = arc4_getword();
    _ARC4_UNLOCK();
    return val;
}

void android_arc4random_buf(void *_buf, size_t n) {
    unsigned char *buf = (unsigned char *)_buf;
    _ARC4_LOCK();
    if (!rs_initialized || arc4_stir_pid != _ARC4_PID) {
        arc4_stir();
    }
    while (n--) {
        if (--arc4_count <= 0) {
            arc4_stir();
        }
        buf[n] = arc4_getbyte();
    }
    _ARC4_UNLOCK();
}

uint32_t android_arc4random_uniform(uint32_t upper_bound) {
    uint32_t r, min;
    if (upper_bound < 2) {
        return 0;
    }
#if (ULONG_MAX > 0xffffffffUL)
    min = 0x100000000UL % upper_bound;
#else
    if (upper_bound > 0x80000000) {
        min = 1 + ~upper_bound;
    } else {
        min = ((0xffffffff - (upper_bound * 2)) + 1) % upper_bound;
    }
#endif
    for (;;) {
        r = android_arc4random();
        if (r >= min) {
            break;
        }
    }
    return r % upper_bound;
}
