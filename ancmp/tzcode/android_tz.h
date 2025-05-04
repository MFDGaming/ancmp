#ifndef ANCMP_ANDROID_TZ_H
#define ANCMP_ANDROID_TZ_H

#include <limits.h>

extern char *android_tzname[2];

typedef struct {
    char    tzh_magic[4];
    char    tzh_version[1];
    char    tzh_reserved[15];
    char    tzh_ttisgmtcnt[4];
    char    tzh_ttisstdcnt[4];
    char    tzh_leapcnt[4];
    char    tzh_timecnt[4];
    char    tzh_typecnt[4];
    char    tzh_charcnt[4];
} android_tzhead_t;

#define ANDROID_TZ_MAGIC    "TZif"
#define ANDROID_TZ_MAX_TIMES    1200
#define ANDROID_TZ_MAX_TYPES    256
#define ANDROID_TZ_MAX_CHARS    50
#define ANDROID_TZ_MAX_LEAPS    50
#define ANDROID_SECSPERMIN  60
#define ANDROID_MINSPERHOUR 60
#define ANDROID_HOURSPERDAY 24
#define ANDROID_DAYSPERWEEK 7
#define ANDROID_DAYSPERNYEAR    365
#define ANDROID_DAYSPERLYEAR    366
#define ANDROID_SECSPERHOUR (ANDROID_SECSPERMIN * ANDROID_MINSPERHOUR)
#define ANDROID_SECSPERDAY  ((long) ANDROID_SECSPERHOUR * ANDROID_HOURSPERDAY)
#define ANDROID_MONSPERYEAR 12
#define ANDROID_TM_SUNDAY   0
#define ANDROID_TM_MONDAY   1
#define ANDROID_TM_TUESDAY  2
#define ANDROID_TM_WEDNESDAY    3
#define ANDROID_TM_THURSDAY 4
#define ANDROID_TM_FRIDAY   5
#define ANDROID_TM_SATURDAY 6
#define ANDROID_TM_JANUARY  0
#define ANDROID_TM_FEBRUARY 1
#define ANDROID_TM_MARCH    2
#define ANDROID_TM_APRIL    3
#define ANDROID_TM_MAY      4
#define ANDROID_TM_JUNE     5
#define ANDROID_TM_JULY     6
#define ANDROID_TM_AUGUST   7
#define ANDROID_TM_SEPTEMBER    8
#define ANDROID_TM_OCTOBER  9
#define ANDROID_TM_NOVEMBER 10
#define ANDROID_TM_DECEMBER 11
#define ANDROID_TM_YEAR_BASE    1900
#define ANDROID_EPOCH_YEAR  1970
#define ANDROID_EPOCH_WDAY  ANDROID_TM_THURSDAY
#define android_isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))
#define android_isleap_sum(a, b)    android_isleap((a) % 400 + (b) % 400)

#define ANDROID_TYPE_BIT(type)  (sizeof (type) * CHAR_BIT)
#define ANDROID_TYPE_SIGNED(type) (((type) -1) < 0)

#define ANDROID_INT_STRLEN_MAXIMUM(type) \
    ((ANDROID_TYPE_BIT(type) - ANDROID_TYPE_SIGNED(type)) * 302 / 1000 + \
    1 + ANDROID_TYPE_SIGNED(type))

#endif
