#include "android_sprint.h"
#include <limits.h>

#ifdef _WIN32

int android_vsnprintf(char *str, size_t n, const char *fmt, va_list ap) {
	char dummy;
    int ret = 0;
    va_list ap_copy;
	if (n > INT_MAX) {
		n = INT_MAX;
    }
	if (n == 0) {
		str = &dummy;
		n = 1;
	}
#if defined(_MSC_VER) && !defined(va_copy)
    ap_copy = ap;
#else
    va_copy(ap_copy, ap);
#endif
    ret = _vsnprintf(NULL, 0, fmt, ap);
#if !defined(_MSC_VER) || defined(va_copy)
    va_end(ap_copy);
#endif
    if (ret == -1) {
        str[0] = '\0';
        return 0;
    }
    _vsnprintf(str, n, fmt, ap);
    if ((ret + 1) > n) {
	    str[n - 1] = '\0';
    } else {
        str[ret] = '\0';
    }
	return ret;
}

int android_snprintf(char *str, size_t n, const char *fmt, ...) {
	va_list ap;
	int ret;
    va_start(ap, fmt);
    ret = android_vsnprintf(str, n, fmt, ap);
    va_end(ap);
	return ret;
}

int android_vsprintf(char *str, const char *fmt, va_list ap) {
    return android_vsnprintf(str, INT_MAX, fmt, ap);
}

int android_sprintf(char *str, const char *fmt, ...) {
    va_list ap;
	int ret;
    va_start(ap, fmt);
    ret = android_vsnprintf(str, INT_MAX, fmt, ap);
    va_end(ap);
	return ret;
}

#endif