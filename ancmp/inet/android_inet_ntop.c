/*	$OpenBSD: inet_ntop.c,v 1.7 2005/08/06 20:30:03 espie Exp $	*/
/* Copyright (c) 1996 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#include "android_inet_ntop.h"
#include <errno.h>
#include "../android_socket.h"
#include <stdio.h>
#include <string.h>
#include "../string/android_string.h"

static const char *android_inet_ntop4(const unsigned char *src, char *dst, size_t size) {
	static const char fmt[] = "%u.%u.%u.%u";
	char tmp[sizeof("255.255.255.255")];
	int l;
	l = snprintf(tmp, size, fmt, src[0], src[1], src[2], src[3]);
	if (l <= 0 || (size_t)l >= size) {
		errno = ENOSPC;
		return (NULL);
	}
	android_strlcpy(dst, tmp, size);
	return (dst);
}

static const char *android_inet_ntop6(const unsigned char *src, char *dst, size_t size) {
	char tmp[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"];
	char *tp, *ep;
	struct { int base, len; } best, cur;
	unsigned int words[16 / 2];
	int i;
	int advance;

	android_memset(words, '\0', sizeof words);
	for (i = 0; i < 16; i++)
		words[i / 2] |= (src[i] << ((1 - (i % 2)) << 3));
	best.base = -1;
	best.len  = 0;
	cur.base = -1;
	cur.len  = 0;
	for (i = 0; i < (16 / 2); i++) {
		if (words[i] == 0) {
			if (cur.base == -1)
				cur.base = i, cur.len = 1;
			else
				cur.len++;
		} else {
			if (cur.base != -1) {
				if (best.base == -1 || cur.len > best.len)
					best = cur;
				cur.base = -1;
			}
		}
	}
	if (cur.base != -1) {
		if (best.base == -1 || cur.len > best.len)
			best = cur;
	}
	if (best.base != -1 && best.len < 2)
		best.base = -1;
	tp = tmp;
	ep = tmp + sizeof(tmp);
	for (i = 0; i < (16 / 2) && tp < ep; i++) {
		if (best.base != -1 && i >= best.base &&
		    i < (best.base + best.len)) {
			if (i == best.base) {
				if (tp + 1 >= ep)
					return (NULL);
				*tp++ = ':';
			}
			continue;
		}

		if (i != 0) {
			if (tp + 1 >= ep)
				return (NULL);
			*tp++ = ':';
		}

		if (i == 6 && best.base == 0 &&
		    (best.len == 6 || (best.len == 5 && words[5] == 0xffff))) {
			if (!android_inet_ntop4(src+12, tp, (size_t)(ep - tp)))
				return (NULL);
			tp += android_strlen(tp);
			break;
		}
		advance = snprintf(tp, ep - tp, "%x", words[i]);
		if (advance <= 0 || advance >= ep - tp)
			return (NULL);
		tp += advance;
	}

	if (best.base != -1 && (best.base + best.len) == (16 / 2)) {
		if (tp + 1 >= ep)
			return (NULL);
		*tp++ = ':';
	}
	if (tp + 1 >= ep)
		return (NULL);
	*tp++ = '\0';

	if ((size_t)(tp - tmp) > size) {
		errno = ENOSPC;
		return (NULL);
	}
	android_strlcpy(dst, tmp, size);
	return (dst);
}

const char *android_inet_ntop(int af, const void *src, char *dst, size_t size) {
	switch (af) {
	case ANDROID_AF_INET:
		return (android_inet_ntop4(src, dst, size));
	case ANDROID_AF_INET6:
		return (android_inet_ntop6(src, dst, size));
	default:
		errno = EAFNOSUPPORT;
		return (NULL);
	}
	/* NOTREACHED */
}