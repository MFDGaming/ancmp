#ifndef ANCMP_ANDROID_TIME
#define ANCMP_ANDROID_TIME

#include <time.h>

#ifdef _WIN32
#define ANDROID_CLOCK_REALTIME 0
#define ANDROID_CLOCK_MONOTONIC 1

typedef long android_time_t;

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

typedef struct {
    int     tm_sec;
    int     tm_min;
    int     tm_hour;
    int     tm_mday;
    int     tm_mon;
    int     tm_year;
    int     tm_wday;
    int     tm_yday;
    int     tm_isdst;
    long int tm_gmtoff;
    const char *tm_zone;
 } android_tm_t;

typedef int android_clockid_t;

struct tm *android_localtime_r(const long *timep, struct tm *result);

struct tm *android_localtime(const android_time_t *timep);

int android_clock_gettime(android_clockid_t clk_id, android_timespec_t *tp);

int android_gettimeofday(android_timeval_t *tp, android_timezone_t *tzp);

int android_nanosleep(const android_timespec_t *ts, android_timespec_t *rem);

android_time_t android_time(android_time_t *tloc);

android_tm_t *android_gmtime(const android_time_t *timer);

android_time_t android_mktime(struct tm *timeptr);

#else
#include <sys/time.h>

#define android_time_t time_t
#define android_clockid_t clockid_t
#define android_timespec_t struct timespec
#define android_timeval_t struct timeval
#define android_timezone_t struct timezone

#define ANDROID_CLOCK_REALTIME CLOCK_REALTIME
#define ANDROID_CLOCK_MONOTONIC CLOCK_MONOTONIC

#define android_localtime_r localtime_r
#define android_localtime localtime
#define android_clock_gettime clock_gettime
#define android_gettimeofday gettimeofday
#define android_nanosleep nanosleep
#define android_time time
#define android_gmtime gmtime
#define android_mktime mktime
#endif

typedef struct {
    android_time_t  time;
    unsigned short  millitm;
    short           timezone;
    short           dstflag;
} android_timeb_t;

int android_ftime(android_timeb_t *tp);

int android_usleep(unsigned long usec);

#endif
