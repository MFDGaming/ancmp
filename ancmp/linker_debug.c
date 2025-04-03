#include "linker_debug.h"
#include <stdarg.h>

void PRINT(const char *fmt, ...) {
#if LINKER_DEBUG
    va_list ap;
    va_start(ap, fmt);
    printf("[linker] : ");
    vprintf(fmt, ap);
    va_end(ap);
#endif
}

void INFO(const char *fmt, ...) {
#if LINKER_DEBUG
    va_list ap;
    va_start(ap, fmt);
    printf("[linker] INFO : ");
    vprintf(fmt, ap);
    va_end(ap);
#endif
}

void TRACE(const char *fmt, ...) {
#if LINKER_DEBUG
    va_list ap;
    va_start(ap, fmt);
    printf("[linker] TRACE : ");
    vprintf(fmt, ap);
    va_end(ap);
#endif
}

void WARN(const char *fmt, ...) {
#if LINKER_DEBUG && !TIMING
    va_list ap;
    va_start(ap, fmt);
    printf("[linker] WARN : ");
    vprintf(fmt, ap);
    va_end(ap);
#endif
}

void ERROR_O(const char *fmt, ...) {
#if LINKER_DEBUG
    va_list ap;
    va_start(ap, fmt);
    printf("[linker] ERROR : ");
    vprintf(fmt, ap);
    va_end(ap);
#endif
}

void DEBUG(const char *fmt, ...) {
#if LINKER_DEBUG && TRACE_DEBUG
    va_list ap;
    va_start(ap, fmt);
    printf("[linker] DEBUG : ");
    vprintf(fmt, ap);
    va_end(ap);
#endif
}

void TRACE_TYPE(int type, const char *fmt, ...) {
#if LINKER_DEBUG && TRACE_DEBUG
    va_list ap;
    va_start(ap, fmt);
    if (type == RELO && DO_TRACE_RELO) {
        printf("[linker] TRACE : ");
        vprintf(fmt, ap);
    } else if (type == LOOKUP && DO_TRACE_LOOKUP) {
        printf("[linker] TRACE : ");
        vprintf(fmt, ap);
    }
    va_end(ap);
#endif
}


