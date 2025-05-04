#include "android_strto.h"
#include <errno.h>
#include "android_ctype.h"

uintmax_t android_strtoumax(const char *nptr, char **endptr, int base) {
	const char *s;
	uintmax_t acc, cutoff;
	int c;
	int neg, any, cutlim;
	/*
	 * See strtoq for comments as to the logic used.
	 */
	s = nptr;
	do {
		c = (unsigned char) *s++;
	} while (android_isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else {
		neg = 0;
		if (c == '+')
			c = *s++;
	}
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
        /* BIONIC: avoid division and modulo for common cases */
#define  CASE_BASE(x)                            \
            case x: cutoff = UINTMAX_MAX / x;    \
	            cutlim = UINTMAX_MAX % x;    \
		    break
        switch (base) {
        CASE_BASE(8);
	CASE_BASE(10);
	CASE_BASE(16);
	default:
	    cutoff = UINTMAX_MAX / base;
	    cutlim = UINTMAX_MAX % base;
	}
	for (acc = 0, any = 0;; c = (unsigned char) *s++) {
		if (android_isdigit(c))
			c -= '0';
		else if (android_isalpha(c))
			c -= android_isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0)
			continue;
		if (acc > cutoff || (acc == cutoff && c > cutlim)) {
			any = -1;
			acc = UINTMAX_MAX;
			errno = ERANGE;
		} else {
			any = 1;
			acc *= (uintmax_t)base;
			acc += c;
		}
	}
	if (neg && any > 0)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
	return (acc);
}

uint64_t android_strtoull(const char *nptr, char **endptr, int base) {
    return (uint64_t)android_strtoumax(nptr, endptr, base);
}
