#ifndef ANCMP_ANDROID_LOGD_H
#define ANCMP_ANDROID_LOGD_H

#include <stdarg.h>

enum  {
    ANDROID_LOG_UNKNOWN = 0,
    ANDROID_LOG_DEFAULT,
    ANDROID_LOG_VERBOSE,
    ANDROID_LOG_DEBUG,
    ANDROID_LOG_INFO,
    ANDROID_LOG_WARN,
    ANDROID_LOG_ERROR,
    ANDROID_LOG_FATAL,
    ANDROID_LOG_SILENT,
};

int android_log_write(int prio, const char* tag, const char* buffer);

int android_log_print(int prio, const char *tag, const char *fmt, ...);

int android_log_vprint(int prio, const char *tag, const char *fmt, va_list ap);

#endif