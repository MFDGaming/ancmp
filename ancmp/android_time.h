#ifndef ANCMP_ANDROID_TIME
#define ANCMP_ANDROID_TIME

#include <time.h>

typedef long android_time_t;

typedef struct {
    android_time_t  time;
    unsigned short  millitm;
    short           timezone;
    short           dstflag;
} android_timeb_t;

#ifdef _WIN32
#define ANDROID_CLOCK_REALTIME 0
#define ANDROID_CLOCK_MONOTONIC 1

typedef struct {
    android_time_t tv_sec;
    long tv_nsec;
} android_timespec_t;

typedef struct {
    android_time_t tv_sec;
    long tv_usec;
} android_timeval_t;

typedef struct {
    int tz_minuteswest;
    int tz_dsttime;
} android_timezone_t;

typedef int android_clockid_t;

struct tm *android_localtime_r(const long *timep, struct tm *result);

int android_clock_gettime(android_clockid_t clk_id, android_timespec_t *tp);

int android_gettimeofday(android_timeval_t *tp, android_timezone_t *tzp);

int android_nanosleep(const android_timespec_t *ts, android_timespec_t *rem);

#else
#include <sys/time.h>

#define android_clockid_t clockid_t
#define android_timespec_t struct timespec
#define android_timeval_t struct timeval
#define android_timezone_t struct timezone

#define ANDROID_CLOCK_REALTIME CLOCK_REALTIME
#define ANDROID_CLOCK_MONOTONIC CLOCK_MONOTONIC

#define android_localtime_r localtime_r
#define android_clock_gettime clock_gettime
#define android_gettimeofday gettimeofday
#define android_nanosleep nanosleep
#endif

int android_ftime(android_timeb_t *tp);

int android_usleep(unsigned long usec);

#endif