#include "android_wctype.h"
#include "../android_ctype.h"
#include "../string/android_string.h"
#include <stdio.h>
#include "../tzcode/android_strftime.h"

int android_iswalnum(android_wint_t wc) {
    return android_isalnum(wc);
}

int android_iswalpha(android_wint_t wc) {
    return android_isalpha(wc);
}

int android_iswcntrl(android_wint_t wc) {
    return android_iscntrl(wc);
}

int android_iswdigit(android_wint_t wc) {
    return android_isdigit(wc);
}

int android_iswgraph(android_wint_t wc) {
    return android_isgraph(wc);
}

int android_iswlower(android_wint_t wc) {
    return android_islower(wc);
}

int android_iswprint(android_wint_t wc) {
    return android_isprint(wc);
}

int android_iswpunct(android_wint_t wc) {
    return android_ispunct(wc);
}

int android_iswspace(android_wint_t wc) {
    return android_isspace(wc);
}

int android_iswupper(android_wint_t wc) {
    return android_isupper(wc);
}

int android_iswxdigit(android_wint_t wc) {
    return android_isxdigit(wc);
}

int android_iswctype(android_wint_t wc, android_wctype_t charclass) {
    switch (charclass) {
        case ANDROID_WC_TYPE_ALNUM: return android_isalnum(wc);
        case ANDROID_WC_TYPE_ALPHA: return android_isalpha(wc);
        case ANDROID_WC_TYPE_BLANK: return android_isblank(wc);
        case ANDROID_WC_TYPE_CNTRL: return android_iscntrl(wc);
        case ANDROID_WC_TYPE_DIGIT: return android_isdigit(wc);
        case ANDROID_WC_TYPE_GRAPH: return android_isgraph(wc);
        case ANDROID_WC_TYPE_LOWER: return android_islower(wc);
        case ANDROID_WC_TYPE_PRINT: return android_isprint(wc);
        case ANDROID_WC_TYPE_PUNCT: return android_ispunct(wc);
        case ANDROID_WC_TYPE_SPACE: return android_isspace(wc);
        case ANDROID_WC_TYPE_UPPER: return android_isupper(wc);
        case ANDROID_WC_TYPE_XDIGIT: return android_isxdigit(wc);
        default: return 0;
    };
}

android_wctype_t android_wctype(const char *property)
{
    static const char* const  properties[ANDROID_WC_TYPE_MAX] =
    {
        "<invalid>",
        "alnum", "alpha", "blank", "cntrl", "digit", "graph",
        "lower", "print", "punct", "space", "upper", "xdigit"
    };
    int  nn;
    for ( nn = 0; nn < ANDROID_WC_TYPE_MAX; nn++ )
        if ( !android_strcmp( properties[nn], property ) )
            return (android_wctype_t)(nn);
    return 0;
}

int android_wcwidth(android_wchar_t wc) {
    return (wc > 0);
}

android_wint_t android_towlower(android_wint_t wc) {
    return (android_wint_t)android_tolower(wc);
}

android_wint_t android_towupper(android_wint_t wc) {
    return (android_wint_t)android_toupper(wc);
}

int android_wctob(android_wint_t c) {
    return c;
}

android_wint_t android_btowc(int c) {
    return (c == EOF) ? ANDROID_WEOF : c;
}

size_t android_wcrtomb(char *s, android_wchar_t wc, android_mbstate_t *ps) {
    if (s != NULL)
        *s = 1;
    return 1;
}

size_t android_mbrtowc(android_wchar_t *pwc, const char *s, size_t n, android_mbstate_t *ps) {
    if (s == NULL) {
        s   = "";
        pwc = NULL;
    }
    if (n == 0) {
        if (pwc)
            *pwc = 0;
        return 0;
    }
    if (pwc)
        *pwc = *s;
    return (*s != 0);
}

size_t android_wcsftime(android_wchar_t *wcs, size_t maxsize, const android_wchar_t *format,  const struct tm *timptr) {
    return android_strftime((char*)wcs, maxsize, (const char*)format, timptr);
}
