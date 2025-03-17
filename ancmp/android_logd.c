#include "android_logd.h"
#include <stdio.h>

static char *prio_to_text[] = {
    "UNKNOWN",
    "DEFAULT",
    "VERBOSE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
    "SILENT"
};

int android_log_write(int prio, const char* tag, const char* buffer) {
    printf("[%s] %s: %s\n", tag, prio_to_text[prio % 9], buffer);
    return 0;
}

int android_log_print(int prio, const char *tag, const char *fmt, ...) {
    printf("[%s] %s: ", tag, prio_to_text[prio % 9]);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    return 0;
}

int android_log_vprint(int prio, const char *tag, const char *fmt, va_list ap) {
    printf("[%s] %s: ", tag, prio_to_text[prio % 9]);
    vprintf(fmt, ap);
    printf("\n");
    return 0;
}