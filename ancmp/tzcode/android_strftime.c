/*
** Copyright (c) 1989 The Regents of the University of California.
** All rights reserved.
**
** Redistribution and use in source and binary forms are permitted
** provided that the above copyright notice and this paragraph are
** duplicated in all such forms and that any documentation,
** advertising materials, and other materials related to such
** distribution and use acknowledge that the software was developed
** by the University of California, Berkeley. The name of the
** University may not be used to endorse or promote products derived
** from this software without specific prior written permission.
** THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "android_strftime.h"
#include "android_tz.h"
#include <stdlib.h>
#include "../android_sprint.h"
#include "../android_time.h"
#include "../ancmp_stdint.h"
#include "../android_ctype.h"

typedef int64_t android_time64_t;

struct lc_time_T {
    const char *    mon[ANDROID_MONSPERYEAR];
    const char *    month[ANDROID_MONSPERYEAR];
    const char *    standalone_month[ANDROID_MONSPERYEAR];
    const char *    wday[ANDROID_DAYSPERWEEK];
    const char *    weekday[ANDROID_DAYSPERWEEK];
    const char *    X_fmt;
    const char *    x_fmt;
    const char *    c_fmt;
    const char *    am;
    const char *    pm;
    const char *    date_fmt;
};

#define Locale  (&C_time_locale)
static const struct lc_time_T   C_time_locale = {
    {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    }, {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    }, {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    }, {
        "Sun", "Mon", "Tue", "Wed",
        "Thu", "Fri", "Sat"
    }, {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    },
    "%H:%M:%S",
    "%m/%d/%y",
    "%a %b %e %T %Y",
    "AM",
    "PM",
    "%a %b %e %H:%M:%S %Z %Y"
};

#define IN_NONE 0
#define IN_SOME 1
#define IN_THIS 2
#define IN_ALL  3
#define FORCE_LOWER_CASE 0x100

static char *getformat(int modifier, char *normal, char *underscore, char *dash, char *zero) {
    switch (modifier) {
    case '_':
        return underscore;
    case '-':
        return dash;
    case '0':
        return zero;
    }
    return normal;
}

static char *_add(const char *str, char *pt, char *ptlim, int modifier) {
        int c;
        switch (modifier) {
        case FORCE_LOWER_CASE:
                while (pt < ptlim && (*pt = android_tolower(*str++)) != '\0') {
                        ++pt;
                }
                break;
        case '^':
                while (pt < ptlim && (*pt = android_toupper(*str++)) != '\0') {
                        ++pt;
                }
                break;
        case '#':
                while (pt < ptlim && (c = *str++) != '\0') {
                        if (android_isupper(c)) {
                                c = android_tolower(c);
                        } else if (android_islower(c)) {
                                c = android_toupper(c);
                        }
                        *pt = c;
                        ++pt;
                }
                break;
        default:
                while (pt < ptlim && (*pt = *str++) != '\0') {
                        ++pt;
                }
        }
    return pt;
}

static char *_conv(int n, char *format, char *pt, char *ptlim) {
    char    buf[ANDROID_INT_STRLEN_MAXIMUM(int) + 1];
    (void) android_snprintf(buf, sizeof(buf), format, n);
    return _add(buf, pt, ptlim, 0);
}

static char *_yconv(int a, int b, int convert_top, int convert_yy, char *pt, char *ptlim, int modifier) {
    register int    lead;
    register int    trail;
#define DIVISOR 100
    trail = a % DIVISOR + b % DIVISOR;
    lead = a / DIVISOR + b / DIVISOR + trail / DIVISOR;
    trail %= DIVISOR;
    if (trail < 0 && lead > 0) {
        trail += DIVISOR;
        --lead;
    } else if (lead < 0 && trail > 0) {
        trail -= DIVISOR;
        ++lead;
    }
    if (convert_top) {
        if (lead == 0 && trail < 0)
            pt = _add("-0", pt, ptlim, modifier);
        else    pt = _conv(lead, getformat(modifier, "%02d",
                                                   "%2d", "%d", "%02d"),
                                   pt, ptlim);
    }
    if (convert_yy)
        pt = _conv(((trail < 0) ? -trail : trail),
                           getformat(modifier, "%02d", "%2d", "%d", "%02d"),
                           pt, ptlim);
    return pt;
}

static char *_fmt(const char *format, const struct tm *t, char *pt, char *ptlim, int *warnp, const struct lc_time_T *locale) {
    for ( ; *format; ++format) {
        if (*format == '%') {
            int modifier = 0;
label:
            switch (*++format) {
            case '\0':
                --format;
                break;
            case 'A':
                pt = _add((t->tm_wday < 0 ||
                    t->tm_wday >= ANDROID_DAYSPERWEEK) ?
                    "?" : locale->weekday[t->tm_wday],
                    pt, ptlim, modifier);
                continue;
            case 'a':
                pt = _add((t->tm_wday < 0 ||
                    t->tm_wday >= ANDROID_DAYSPERWEEK) ?
                    "?" : locale->wday[t->tm_wday],
                    pt, ptlim, modifier);
                continue;
            case 'B':
                if (modifier == '-') {
                    pt = _add((t->tm_mon < 0 ||
                                t->tm_mon >= ANDROID_MONSPERYEAR) ?
                                "?" : locale->standalone_month[t->tm_mon],
                                pt, ptlim, modifier);
                } else {
                    pt = _add((t->tm_mon < 0 ||
                                t->tm_mon >= ANDROID_MONSPERYEAR) ?
                                "?" : locale->month[t->tm_mon],
                                pt, ptlim, modifier);
                }
                continue;
            case 'b':
            case 'h':
                pt = _add((t->tm_mon < 0 ||
                    t->tm_mon >= ANDROID_MONSPERYEAR) ?
                    "?" : locale->mon[t->tm_mon],
                    pt, ptlim, modifier);
                continue;
            case 'C':
                /*
                ** %C used to do a...
                **  _fmt("%a %b %e %X %Y", t);
                ** ...whereas now POSIX 1003.2 calls for
                ** something completely different.
                ** (ado, 1993-05-24)
                */
                pt = _yconv(t->tm_year, ANDROID_TM_YEAR_BASE, 1, 0,
                    pt, ptlim, modifier);
                continue;
            case 'c':
                {
                int warn2 = IN_SOME;
                pt = _fmt(locale->c_fmt, t, pt, ptlim, warnp, locale);
                if (warn2 == IN_ALL)
                    warn2 = IN_THIS;
                if (warn2 > *warnp)
                    *warnp = warn2;
                }
                continue;
            case 'D':
                                pt = _fmt("%m/%d/%y", t, pt, ptlim, warnp, locale);
                continue;
            case 'd':
                                pt = _conv(t->tm_mday,
                                           getformat(modifier, "%02d",
                                                     "%2d", "%d", "%02d"),
                                           pt, ptlim);
                continue;
            case 'E':
            case 'O':
                /*
                ** C99 locale modifiers.
                ** The sequences
                **  %Ec %EC %Ex %EX %Ey %EY
                **  %Od %oe %OH %OI %Om %OM
                **  %OS %Ou %OU %OV %Ow %OW %Oy
                ** are supposed to provide alternate
                ** representations.
                */
                goto label;
            case '_':
            case '-':
            case '0':
            case '^':
            case '#':
                modifier = *format;
                goto label;
            case 'e':
                pt = _conv(t->tm_mday,
                                           getformat(modifier, "%2d",
                                                     "%2d", "%d", "%02d"),
                                           pt, ptlim);
                continue;
            case 'F':
                pt = _fmt("%Y-%m-%d", t, pt, ptlim, warnp, locale);
                continue;
            case 'H':
                pt = _conv(t->tm_hour,
                                           getformat(modifier, "%02d",
                                                     "%2d", "%d", "%02d"),
                                           pt, ptlim);
                continue;
            case 'I':
                pt = _conv((t->tm_hour % 12) ?
                    (t->tm_hour % 12) : 12,
                    getformat(modifier, "%02d",
                                                  "%2d", "%d", "%02d"),
                                        pt, ptlim);
                continue;
            case 'j':
                pt = _conv(t->tm_yday + 1,
                           getformat(modifier, "%03d", "%3d", "%d", "%03d"),
                           pt, ptlim);
                continue;
            case 'k':
                /*
                ** This used to be...
                **  _conv(t->tm_hour % 12 ?
                **      t->tm_hour % 12 : 12, 2, ' ');
                ** ...and has been changed to the below to
                ** match SunOS 4.1.1 and Arnold Robbins'
                ** strftime version 3.0. That is, "%k" and
                ** "%l" have been swapped.
                ** (ado, 1993-05-24)
                */
                pt = _conv(t->tm_hour,
                                           getformat(modifier, "%2d",
                                                     "%2d", "%d", "%02d"),
                                           pt, ptlim);
                continue;
#ifdef KITCHEN_SINK
            case 'K':
                /*
                ** After all this time, still unclaimed!
                */
                pt = _add("kitchen sink", pt, ptlim, modifier);
                continue;
#endif /* defined KITCHEN_SINK */
            case 'l':
                /*
                ** This used to be...
                **  _conv(t->tm_hour, 2, ' ');
                ** ...and has been changed to the below to
                ** match SunOS 4.1.1 and Arnold Robbin's
                ** strftime version 3.0. That is, "%k" and
                ** "%l" have been swapped.
                ** (ado, 1993-05-24)
                */
                pt = _conv((t->tm_hour % 12) ?
                    (t->tm_hour % 12) : 12,
                    getformat(modifier, "%2d",
                                                  "%2d", "%d", "%02d"),
                                        pt, ptlim);
                continue;
            case 'M':
                pt = _conv(t->tm_min,
                                           getformat(modifier, "%02d",
                                                     "%2d", "%d", "%02d"),
                                           pt, ptlim);
                continue;
            case 'm':
                pt = _conv(t->tm_mon + 1,
                                           getformat(modifier, "%02d",
                                                     "%2d", "%d", "%02d"),
                                           pt, ptlim);
                continue;
            case 'n':
                pt = _add("\n", pt, ptlim, modifier);
                continue;
            case 'p':
                pt = _add((t->tm_hour >= (ANDROID_HOURSPERDAY / 2)) ?
                    locale->pm :
                    locale->am,
                    pt, ptlim, modifier);
                continue;
            case 'P':
                pt = _add((t->tm_hour >= (ANDROID_HOURSPERDAY / 2)) ?
                    locale->pm :
                    locale->am,
                    pt, ptlim, FORCE_LOWER_CASE);
                continue;
            case 'R':
                pt = _fmt("%H:%M", t, pt, ptlim, warnp, locale);
                continue;
            case 'r':
                pt = _fmt("%I:%M:%S %p", t, pt, ptlim, warnp, locale);
                continue;
            case 'S':
                pt = _conv(t->tm_sec,
                                           getformat(modifier, "%02d",
                                                     "%2d", "%d", "%02d"),
                                           pt, ptlim);
                continue;
            case 's':
                {
                    struct tm   tm;
                    char        buf[ANDROID_INT_STRLEN_MAXIMUM(
                                android_time64_t) + 1];
                    android_time64_t    mkt;
                    tm = *t;
                    mkt = (android_time64_t)mktime(&tm);
                    if (ANDROID_TYPE_SIGNED(android_time64_t))
                        (void) android_snprintf(buf, sizeof(buf), "%lld",
                            (int64_t) mkt);
                    else    (void) android_snprintf(buf, sizeof(buf), "%llu",
                            (uint64_t) mkt);
                    pt = _add(buf, pt, ptlim, modifier);
                }
                continue;
            case 'T':
                pt = _fmt("%H:%M:%S", t, pt, ptlim, warnp, locale);
                continue;
            case 't':
                pt = _add("\t", pt, ptlim, modifier);
                continue;
            case 'U':
                pt = _conv((t->tm_yday + ANDROID_DAYSPERWEEK -
                    t->tm_wday) / ANDROID_DAYSPERWEEK,
                    getformat(modifier, "%02d",
                                                  "%2d", "%d", "%02d"),
                                        pt, ptlim);
                continue;
            case 'u':
                /*
                ** From Arnold Robbins' strftime version 3.0:
                ** "ISO 8601: Weekday as a decimal number
                ** [1 (Monday) - 7]"
                ** (ado, 1993-05-24)
                */
                pt = _conv((t->tm_wday == 0) ?
                ANDROID_DAYSPERWEEK : t->tm_wday, "%d", pt, ptlim);
                continue;
            case 'V':   /* ISO 8601 week number */
            case 'G':   /* ISO 8601 year (four digits) */
            case 'g':   /* ISO 8601 year (two digits) */
/*
** From Arnold Robbins' strftime version 3.0: "the week number of the
** year (the first Monday as the first day of week 1) as a decimal number
** (01-53)."
** (ado, 1993-05-24)
**
** From "http://www.ft.uni-erlangen.de/~mskuhn/iso-time.html" by Markus Kuhn:
** "Week 01 of a year is per definition the first week which has the
** Thursday in this year, which is equivalent to the week which contains
** the fourth day of January. In other words, the first week of a new year
** is the week which has the majority of its days in the new year. Week 01
** might also contain days from the previous year and the week before week
** 01 of a year is the last week (52 or 53) of the previous year even if
** it contains days from the new year. A week starts with Monday (day 1)
** and ends with Sunday (day 7). For example, the first week of the year
** 1997 lasts from 1996-12-30 to 1997-01-05..."
** (ado, 1996-01-02)
*/
                {
                    int year;
                    int base;
                    int yday;
                    int wday;
                    int w;
                    year = t->tm_year;
                    base = ANDROID_TM_YEAR_BASE;
                    yday = t->tm_yday;
                    wday = t->tm_wday;
                    for ( ; ; ) {
                        int len;
                        int bot;
                        int top;
                        len = android_isleap_sum(year, base) ?
                            ANDROID_DAYSPERLYEAR :
                            ANDROID_DAYSPERNYEAR;
                        /*
                        ** What yday (-3 ... 3) does
                        ** the ISO year begin on?
                        */
                        bot = ((yday + 11 - wday) %
                            ANDROID_DAYSPERWEEK) - 3;
                        /*
                        ** What yday does the NEXT
                        ** ISO year begin on?
                        */
                        top = bot -
                            (len % ANDROID_DAYSPERWEEK);
                        if (top < -3)
                            top += ANDROID_DAYSPERWEEK;
                        top += len;
                        if (yday >= top) {
                            ++base;
                            w = 1;
                            break;
                        }
                        if (yday >= bot) {
                            w = 1 + ((yday - bot) /
                                ANDROID_DAYSPERWEEK);
                            break;
                        }
                        --base;
                        yday += android_isleap_sum(year, base) ?
                            ANDROID_DAYSPERLYEAR :
                            ANDROID_DAYSPERNYEAR;
                    }
#ifdef XPG4_1994_04_09
                    if ((w == 52 &&
                        t->tm_mon == TM_JANUARY) ||
                        (w == 1 &&
                        t->tm_mon == TM_DECEMBER))
                            w = 53;
#endif /* defined XPG4_1994_04_09 */
                    if (*format == 'V')
                        pt = _conv(w,
                                                           getformat(modifier,
                                                                     "%02d",
                                                                     "%2d",
                                                                     "%d",
                                                                     "%02d"),
                               pt, ptlim);
                    else if (*format == 'g') {
                        *warnp = IN_ALL;
                        pt = _yconv(year, base, 0, 1,
                            pt, ptlim, modifier);
                    } else  pt = _yconv(year, base, 1, 1,
                            pt, ptlim, modifier);
                }
                continue;
            case 'v':
                /*
                ** From Arnold Robbins' strftime version 3.0:
                ** "date as dd-bbb-YYYY"
                ** (ado, 1993-05-24)
                */
                pt = _fmt("%e-%b-%Y", t, pt, ptlim, warnp, locale);
                continue;
            case 'W':
                pt = _conv((t->tm_yday + ANDROID_DAYSPERWEEK -
                    (t->tm_wday ?
                    (t->tm_wday - 1) :
                    (ANDROID_DAYSPERWEEK - 1))) / ANDROID_DAYSPERWEEK,
                    getformat(modifier, "%02d",
                                                  "%2d", "%d", "%02d"),
                                        pt, ptlim);
                continue;
            case 'w':
                pt = _conv(t->tm_wday, "%d", pt, ptlim);
                continue;
            case 'X':
                pt = _fmt(locale->X_fmt, t, pt, ptlim, warnp, locale);
                continue;
            case 'x':
                {
                int warn2 = IN_SOME;
                pt = _fmt(locale->x_fmt, t, pt, ptlim, &warn2, locale);
                if (warn2 == IN_ALL)
                    warn2 = IN_THIS;
                if (warn2 > *warnp)
                    *warnp = warn2;
                }
                continue;
            case 'y':
                *warnp = IN_ALL;
                pt = _yconv(t->tm_year, ANDROID_TM_YEAR_BASE, 0, 1,
                    pt, ptlim, modifier);
                continue;
            case 'Y':
                pt = _yconv(t->tm_year, ANDROID_TM_YEAR_BASE, 1, 1,
                    pt, ptlim, modifier);
                continue;
            case 'Z':
#ifdef TM_ZONE
                if (t->TM_ZONE != NULL)
                    pt = _add(t->TM_ZONE, pt, ptlim,
                                                  modifier);
                else
#endif /* defined TM_ZONE */
                if (t->tm_isdst >= 0)
                    pt = _add(android_tzname[t->tm_isdst != 0],
                        pt, ptlim, modifier);
                /*
                ** C99 says that %Z must be replaced by the
                ** empty string if the time zone is not
                ** determinable.
                */
                continue;
            case 'z':
                {
                int     diff;
                char const *    sign;
                if (t->tm_isdst < 0)
                    continue;
#ifdef TM_GMTOFF
                diff = t->TM_GMTOFF;
#else /* !defined TM_GMTOFF */
                /*
                ** C99 says that the UTC offset must
                ** be computed by looking only at
                ** tm_isdst. This requirement is
                ** incorrect, since it means the code
                ** must rely on magic (in this case
                ** altzone and timezone), and the
                ** magic might not have the correct
                ** offset. Doing things correctly is
                ** tricky and requires disobeying C99;
                ** see GNU C strftime for details.
                ** For now, punt and conform to the
                ** standard, even though it's incorrect.
                **
                ** C99 says that %z must be replaced by the
                ** empty string if the time zone is not
                ** determinable, so output nothing if the
                ** appropriate variables are not available.
                */
                if (t->tm_isdst == 0)
#ifdef USG_COMPAT
                    diff = -timezone;
#else /* !defined USG_COMPAT */
                    continue;
#endif /* !defined USG_COMPAT */
                else
#ifdef ALTZONE
                    diff = -altzone;
#else /* !defined ALTZONE */
                    continue;
#endif /* !defined ALTZONE */
#endif /* !defined TM_GMTOFF */
                if (diff < 0) {
                    sign = "-";
                    diff = -diff;
                } else  sign = "+";
                pt = _add(sign, pt, ptlim, modifier);
                diff /= ANDROID_SECSPERMIN;
                diff = (diff / ANDROID_MINSPERHOUR) * 100 +
                    (diff % ANDROID_MINSPERHOUR);
                pt = _conv(diff,
                                           getformat(modifier, "%04d",
                                                     "%4d", "%d", "%04d"),
                                           pt, ptlim);
                }
                continue;
            case '+':
                pt = _fmt(locale->date_fmt, t, pt, ptlim,
                    warnp, locale);
                continue;
            case '%':
            /*
            ** X311J/88-090 (4.12.3.5): if conversion char is
            ** undefined, behavior is undefined. Print out the
            ** character itself as printf(3) also does.
            */
            default:
                break;
            }
        }
        if (pt == ptlim)
            break;
        *pt++ = *format;
    }
    return pt;
}

static size_t strftime_tz(char *s, size_t maxsize, const char *format, const struct tm *t, const struct lc_time_T *locale) {
    char *  p;
    int warn;
    tzset();
    warn = IN_NONE;
    p = _fmt(((format == NULL) ? "%c" : format), t, s, s + maxsize, &warn, locale);
    if (p == s + maxsize)
        return 0;
    *p = '\0';
    return p - s;
}

size_t android_strftime(char *s, size_t maxsize, const char *format, const struct tm *t) {
    return strftime_tz(s, maxsize, format, t, Locale);
}
