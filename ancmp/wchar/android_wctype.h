#ifndef ANCMP_ANDROID_WCTYPE
#define ANCMP_ANDROID_WCTYPE

#include "android_wchar_internal.h"
#include <limits.h>
#include <time.h>

typedef enum {
    ANDROID_WC_TYPE_INVALID = 0,
    ANDROID_WC_TYPE_ALNUM,
    ANDROID_WC_TYPE_ALPHA,
    ANDROID_WC_TYPE_BLANK,
    ANDROID_WC_TYPE_CNTRL,
    ANDROID_WC_TYPE_DIGIT,
    ANDROID_WC_TYPE_GRAPH,
    ANDROID_WC_TYPE_LOWER,
    ANDROID_WC_TYPE_PRINT,
    ANDROID_WC_TYPE_PUNCT,
    ANDROID_WC_TYPE_SPACE,
    ANDROID_WC_TYPE_UPPER,
    ANDROID_WC_TYPE_XDIGIT,
    ANDROID_WC_TYPE_MAX
} android_wctype_t;

#define ANDROID_WCHAR_MAX   INT_MAX
#define ANDROID_WCHAR_MIN   INT_MIN
#define ANDROID_WEOF        ((android_wint_t)(-1))

typedef struct { int  dummy; }  android_mbstate_t;

int android_iswalnum(android_wint_t wc);

int android_iswalpha(android_wint_t wc);

int android_iswcntrl(android_wint_t wc);

int android_iswdigit(android_wint_t wc);

int android_iswgraph(android_wint_t wc);

int android_iswlower(android_wint_t wc);

int android_iswprint(android_wint_t wc);

int android_iswpunct(android_wint_t wc);

int android_iswspace(android_wint_t wc);

int android_iswupper(android_wint_t wc);

int android_iswxdigit(android_wint_t wc);

int android_iswctype(android_wint_t wc, android_wctype_t charclass);

android_wctype_t android_wctype(const char *property);

int android_wcwidth(android_wchar_t wc);

android_wint_t android_towlower(android_wint_t wc);

android_wint_t android_towupper(android_wint_t wc);

int android_wctob(android_wint_t c);

android_wint_t android_btowc(int c);

size_t android_wcrtomb(char *s, android_wchar_t wc, android_mbstate_t *ps);

size_t android_mbrtowc(android_wchar_t *pwc, const char *s, size_t n, android_mbstate_t *ps);

size_t android_wcsftime(android_wchar_t *wcs, size_t maxsize, const android_wchar_t *format,  const struct tm *timptr);

#endif
