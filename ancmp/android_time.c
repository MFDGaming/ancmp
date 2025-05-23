#include "android_time.h"
#include "android_atomic.h"

#ifdef _WIN32
#include <windows.h>

static void UnixTimeToFileTime(time_t t, FILETIME *ft) {
    const LONGLONG EPOCH_DIFFERENCE = 116444736000000000;
    LONGLONG ull = ((LONGLONG)t * 10000000) + EPOCH_DIFFERENCE;
    ft->dwLowDateTime = (DWORD)ull;
    ft->dwHighDateTime = (DWORD)(ull >> 32);
}

static int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static const int days_in_months_normal[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const int days_in_months_leap[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

struct tm *android_localtime_r(const android_time_t *timep, struct tm *result) {
    FILETIME ft;
    SYSTEMTIME st_utc, st_local;
    TIME_ZONE_INFORMATION tzi;
    DWORD tz_status;
    LONG total_bias;
    int is_dst;
    ULARGE_INTEGER utc_ull;
    LONGLONG bias_in_100ns;
    int year;
    const int *days_in_months;
    int i;

    if (!timep || !result) {
        return NULL;
    }

    UnixTimeToFileTime(*timep, &ft);
    FileTimeToSystemTime(&ft, &st_utc);

    tz_status = GetTimeZoneInformation(&tzi);
    total_bias = tzi.Bias;
    is_dst = 0;

    if (tz_status == TIME_ZONE_ID_DAYLIGHT && tzi.DaylightDate.wMonth != 0) {
        SYSTEMTIME dst_start, dst_end;
        FILETIME ft_start, ft_end;
        ULARGE_INTEGER dst_start_time, dst_end_time;
        SYSTEMTIME st_base;

        dst_start = tzi.DaylightDate;
        dst_end = tzi.StandardDate;

        if (dst_start.wMonth > 0 && dst_end.wMonth > 0) {
            st_base.wYear = st_utc.wYear;
            st_base.wMonth = dst_start.wMonth;
            st_base.wDay = dst_start.wDay;
            st_base.wDayOfWeek = dst_start.wDayOfWeek;
            st_base.wHour = dst_start.wHour;
            st_base.wMinute = 0;
            st_base.wSecond = 0;
            st_base.wMilliseconds = 0;
            SystemTimeToFileTime(&st_base, &ft_start);

            st_base.wMonth = dst_end.wMonth;
            st_base.wDay = dst_end.wDay;
            st_base.wDayOfWeek = dst_end.wDayOfWeek;
            st_base.wHour = dst_end.wHour;
            SystemTimeToFileTime(&st_base, &ft_end);

            utc_ull.LowPart = ft.dwLowDateTime;
            utc_ull.HighPart = ft.dwHighDateTime;
            dst_start_time.LowPart = ft_start.dwLowDateTime;
            dst_start_time.HighPart = ft_start.dwHighDateTime;
            dst_end_time.LowPart = ft_end.dwLowDateTime;
            dst_end_time.HighPart = ft_end.dwHighDateTime;

            if (utc_ull.QuadPart >= dst_start_time.QuadPart && utc_ull.QuadPart < dst_end_time.QuadPart) {
                total_bias += tzi.DaylightBias;
                is_dst = 1;
            }
        }
    }

    utc_ull.LowPart = ft.dwLowDateTime;
    utc_ull.HighPart = ft.dwHighDateTime;
    bias_in_100ns = (LONGLONG)total_bias * 60 * 10000000;
    utc_ull.QuadPart -= bias_in_100ns;

    ft.dwLowDateTime = utc_ull.LowPart;
    ft.dwHighDateTime = utc_ull.HighPart;
    FileTimeToSystemTime(&ft, &st_local);

    result->tm_sec  = st_local.wSecond;
    result->tm_min  = st_local.wMinute;
    result->tm_hour = st_local.wHour;
    result->tm_mday = st_local.wDay;
    result->tm_mon  = st_local.wMonth - 1;
    result->tm_year = st_local.wYear - 1900;
    result->tm_wday = st_local.wDayOfWeek;

    year = result->tm_year + 1900;
    days_in_months = is_leap_year(year) ? days_in_months_leap : days_in_months_normal;

    result->tm_yday = result->tm_mday - 1;
    for (i = 0; i < result->tm_mon; ++i) {
        result->tm_yday += days_in_months[i];
    }

    result->tm_isdst = is_dst;

    return result;
}

struct tm *android_localtime(const android_time_t *timep) {
    static android_tm_t tm;
    return android_localtime_r(timep, (struct tm *)&tm);
}

int android_clock_gettime(android_clockid_t clk_id, android_timespec_t *tp) {
    LARGE_INTEGER freq = {0};
    LARGE_INTEGER now;
    ULARGE_INTEGER time;
    ULONGLONG seconds, remainder, nanoseconds;
    const ULONGLONG EPOCH_DIFFERENCE = 116444736000000000;
    BOOL qpf_success = QueryPerformanceFrequency(&freq);
    if (!qpf_success) {
        return -1;
    }
    if (clk_id == ANDROID_CLOCK_REALTIME) {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        time.LowPart = ft.dwLowDateTime;
        time.HighPart = ft.dwHighDateTime;
        time.QuadPart -= EPOCH_DIFFERENCE;

        tp->tv_sec = (time_t)(time.QuadPart / 10000000);
        tp->tv_nsec = (long)((time.QuadPart % 10000000) * 100);
    } 
    else if (clk_id == ANDROID_CLOCK_MONOTONIC) {
        if (freq.QuadPart == 0 || !QueryPerformanceCounter(&now)) {
            return -1;
        }
        seconds = now.QuadPart / freq.QuadPart;
        remainder = now.QuadPart % freq.QuadPart;
        nanoseconds = (remainder * 1000000000) / freq.QuadPart;

        tp->tv_sec = (time_t)seconds;
        tp->tv_nsec = (long)nanoseconds;
    } 
    else {
        return -1;
    }
    return 0;
}

int android_gettimeofday(android_timeval_t *tp, android_timezone_t *tzp) {
    FILETIME file_time;
    SYSTEMTIME system_time;
    ULARGE_INTEGER ularge;
    const ULONGLONG EPOCH_DIFFERENCE = 116444736000000000;
    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.LowPart = file_time.dwLowDateTime;
    ularge.HighPart = file_time.dwHighDateTime;
    ularge.QuadPart -= EPOCH_DIFFERENCE;
    tp->tv_sec = (long)(ularge.QuadPart / 10000000);
    tp->tv_usec = (long)((ularge.QuadPart % 10000000) / 10);
    return 0;
}

int android_nanosleep(const android_timespec_t *req, android_timespec_t *rem) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, current;
    LONGLONG requested_ticks;

    QueryPerformanceFrequency(&frequency);
    requested_ticks = (LONGLONG)req->tv_sec * frequency.QuadPart + req->tv_nsec * frequency.QuadPart / 1000000000;
    QueryPerformanceCounter(&start);

    while (1) {
        LONGLONG elapsed_ticks;

        QueryPerformanceCounter(&current);

        elapsed_ticks = current.QuadPart - start.QuadPart;

        if (elapsed_ticks >= requested_ticks) {
            break;
        }

        Sleep(0);
    }

    if (rem != NULL) {
        LONGLONG remaining_ticks = requested_ticks - (current.QuadPart - start.QuadPart);
        rem->tv_sec = (time_t)(remaining_ticks / frequency.QuadPart);
        rem->tv_nsec = (long)((remaining_ticks % frequency.QuadPart) * 1000000000 / frequency.QuadPart);
    }

    return 0;
}

android_time_t android_time(android_time_t *tloc) {
    time_t t = time(NULL);
    if (tloc) *tloc = (android_time_t)t;
    return (android_time_t)t;
}

android_tm_t *android_gmtime(const android_time_t *timer) {
    static android_tm_t tm;
    struct tm *real_tm;
    time_t t = (time_t)*timer;
    real_tm = gmtime(&t);
    tm.tm_sec = real_tm->tm_sec;
    tm.tm_min = real_tm->tm_min;
    tm.tm_hour = real_tm->tm_hour;
    tm.tm_mday = real_tm->tm_mday;
    tm.tm_mon = real_tm->tm_mon;
    tm.tm_year = real_tm->tm_year;
    tm.tm_wday = real_tm->tm_wday;
    tm.tm_yday = real_tm->tm_yday;
    tm.tm_isdst = real_tm->tm_isdst;
    tm.tm_gmtoff = 0;
    tm.tm_zone = "UTC";
    return &tm;
}

android_time_t android_mktime(struct tm *timeptr) {
    time_t t = mktime(timeptr);
    return (android_time_t)t;
}

#endif

int android_ftime(android_timeb_t *tp) {
    android_timeval_t tv;
    struct tm local_tm;
    android_time_t utc_time, local_time;

    if (android_gettimeofday(&tv, NULL) != 0) {
        return -1;
    }
    tp->time = tv.tv_sec;
    tp->millitm = tv.tv_usec / 1000;

    utc_time = tv.tv_sec;
    android_localtime_r(&utc_time, &local_tm);

    local_time = (android_time_t)mktime(&local_tm);

    tp->timezone = (int)((double)(local_time - utc_time) / 60);

    tp->dstflag = local_tm.tm_isdst;
    return 0;
}

int android_usleep(unsigned long usec) {
    android_timespec_t ts;
    ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;
    return android_nanosleep(&ts, NULL);
}
