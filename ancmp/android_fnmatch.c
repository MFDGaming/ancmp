#include "android_fnmatch.h"
#include "android_ctype.h"
#include <stdlib.h>
#include "string/android_string.h"
#include <limits.h>
#ifdef _WIN32
#include <windows.h>
#define ANDROID_PATH_MAX MAX_PATH
#else
#include <unistd.h>
#define ANDROID_PATH_MAX PATH_MAX
#endif

static android_cclass_t cclasses[] = {
	{ "alnum",	android_isalnum },
	{ "alpha",	android_isalpha },
	{ "blank",	android_isblank },
	{ "cntrl",	android_iscntrl },
	{ "digit",	android_isdigit },
	{ "graph",	android_isgraph },
	{ "lower",	android_islower },
	{ "print",	android_isprint },
	{ "punct",	android_ispunct },
	{ "space",	android_isspace },
	{ "upper",	android_isupper },
	{ "xdigit",	android_isxdigit },
	{ NULL, NULL }
};

static int android_classmatch(const char *pattern, char test, int foldcase, const char **ep) {
	android_cclass_t *cc;
	const char *colon;
	size_t len;
	int rval = ANDROID_RANGE_NOMATCH;
	const char * const mismatch = pattern;
	if (*pattern != '[' || pattern[1] != ':') {
		*ep = mismatch;
		return(ANDROID_RANGE_ERROR);
	}
	pattern += 2;
	if ((colon = android_strchr(pattern, ':')) == NULL || colon[1] != ']') {
		*ep = mismatch;
		return(ANDROID_RANGE_ERROR);
	}
	*ep = colon + 2;
	len = (size_t)(colon - pattern);
	if (foldcase && android_strncmp(pattern, "upper:]", 7) == 0)
		pattern = "lower:]";
	for (cc = cclasses; cc->name != NULL; cc++) {
		if (!android_strncmp(pattern, cc->name, len) && cc->name[len] == '\0') {
			if (cc->isctype((unsigned char)test))
				rval = ANDROID_RANGE_MATCH;
			break;
		}
	}
	if (cc->name == NULL) {
		/* invalid character class, treat as normal text */
		*ep = mismatch;
		rval = ANDROID_RANGE_ERROR;
	}
	return(rval);
}
/* Most MBCS/collation/case issues handled here.  Wildcard '*' is not handled.
 * EOS '\0' and the ANDROID_FNM_PATHNAME '/' delimiters are not advanced over, 
 * however the "\/" sequence is advanced to '/'.
 *
 * Both pattern and string are **char to support pointer increment of arbitrary
 * multibyte characters for the given locale, in a later iteration of this code
 */
static int android_fnmatch_ch(const char **pattern, const char **string, int flags) {
    const char * const mismatch = *pattern;
    const int nocase = !!(flags & ANDROID_FNM_CASEFOLD);
    const int escape = !(flags & ANDROID_FNM_NOESCAPE);
    const int slash = !!(flags & ANDROID_FNM_PATHNAME);
    int result = ANDROID_FNM_NOMATCH;
    const char *startch;
    int negate;
    if (**pattern == '[')
    {
        ++*pattern;
        /* Handle negation, either leading ! or ^ operators (never both) */
        negate = ((**pattern == '!') || (**pattern == '^'));
        if (negate)
            ++*pattern;
        /* ']' is an ordinary character at the start of the range pattern */
        if (**pattern == ']')
            goto leadingclosebrace;
        while (**pattern)
        {
            if (**pattern == ']') {
                ++*pattern;
                /* XXX: Fix for MBCS character width */
                ++*string;
                return (result ^ negate);
            }
            if (escape && (**pattern == '\\')) {
                ++*pattern;
                /* Patterns must be terminated with ']', not EOS */
                if (!**pattern)
                    break;
            }
            /* Patterns must be terminated with ']' not '/' */
            if (slash && (**pattern == '/'))
                break;
            /* Match character classes. */
            if (android_classmatch(*pattern, **string, nocase, pattern)
                == ANDROID_RANGE_MATCH) {
                result = 0;
                continue;
            }
leadingclosebrace:
            /* Look at only well-formed range patterns; 
             * "x-]" is not allowed unless escaped ("x-\]")
             * XXX: Fix for locale/MBCS character width
             */
            if (((*pattern)[1] == '-') && ((*pattern)[2] != ']'))
            {
                startch = *pattern;
                *pattern += (escape && ((*pattern)[2] == '\\')) ? 3 : 2;
                /* NOT a properly balanced [expr] pattern, EOS terminated 
                 * or ranges containing a slash in ANDROID_FNM_PATHNAME mode pattern
                 * fall out to to the rewind and test '[' literal code path
                 */
                if (!**pattern || (slash && (**pattern == '/')))
                    break;
                /* XXX: handle locale/MBCS comparison, advance by MBCS char width */
                if ((**string >= *startch) && (**string <= **pattern))
                    result = 0;
                else if (nocase && (android_isupper(**string) || android_isupper(*startch)
                                                      || android_isupper(**pattern))
                            && (android_tolower(**string) >= android_tolower(*startch)) 
                            && (android_tolower(**string) <= android_tolower(**pattern)))
                    result = 0;
                ++*pattern;
                continue;
            }
            /* XXX: handle locale/MBCS comparison, advance by MBCS char width */
            if ((**string == **pattern))
                result = 0;
            else if (nocase && (android_isupper(**string) || android_isupper(**pattern))
                            && (android_tolower(**string) == android_tolower(**pattern)))
                result = 0;
            ++*pattern;
        }
        /* NOT a properly balanced [expr] pattern; Rewind
         * and reset result to test '[' literal
         */
        *pattern = mismatch;
        result = ANDROID_FNM_NOMATCH;
    }
    else if (**pattern == '?') {
        /* Optimize '?' match before unescaping **pattern */
        if (!**string || (slash && (**string == '/')))
            return ANDROID_FNM_NOMATCH;
        result = 0;
        goto android_fnmatch_ch_success;
    }
    else if (escape && (**pattern == '\\') && (*pattern)[1]) {
        ++*pattern;
    }
    /* XXX: handle locale/MBCS comparison, advance by the MBCS char width */
    if (**string == **pattern)
        result = 0;
    else if (nocase && (android_isupper(**string) || android_isupper(**pattern))
                    && (android_tolower(**string) == android_tolower(**pattern)))
        result = 0;
    /* Refuse to advance over trailing slash or nulls
     */
    if (!**string || !**pattern || (slash && ((**string == '/') || (**pattern == '/'))))
        return result;
android_fnmatch_ch_success:
    ++*pattern;
    ++*string;
    return result;
}

int android_fnmatch(const char *pattern, const char *string, int flags) {
    static const char dummystring[2] = {' ', 0};
    const int escape = !(flags & ANDROID_FNM_NOESCAPE);
    const int slash = !!(flags & ANDROID_FNM_PATHNAME);
    const int leading_dir = !!(flags & ANDROID_FNM_LEADING_DIR);
    const char *strendseg;
    const char *dummyptr;
    const char *matchptr;
    int wild;
    /* For '*' wild processing only; surpress 'used before initialization'
     * warnings with dummy initialization values;
     */
    const char *strstartseg = NULL;
    const char *mismatch = NULL;
    int matchlen = 0;
    if (android_strnlen(pattern, ANDROID_PATH_MAX) == ANDROID_PATH_MAX ||
        android_strnlen(string, ANDROID_PATH_MAX) == ANDROID_PATH_MAX)
            return (ANDROID_FNM_NOMATCH);
    if (*pattern == '*')
        goto firstsegment;
    while (*pattern && *string)
    {
        /* Pre-decode "\/" which has no special significance, and
         * match balanced slashes, starting a new segment pattern
         */
        if (slash && escape && (*pattern == '\\') && (pattern[1] == '/'))
            ++pattern;
        if (slash && (*pattern == '/') && (*string == '/')) {
            ++pattern;
            ++string;
        }            
firstsegment:
        /* At the beginning of each segment, validate leading period behavior.
         */
        if ((flags & ANDROID_FNM_PERIOD) && (*string == '.'))
        {
            if (*pattern == '.')
                ++pattern;
            else if (escape && (*pattern == '\\') && (pattern[1] == '.'))
                pattern += 2;
            else
                return ANDROID_FNM_NOMATCH;
            ++string;
        }
        /* Determine the end of string segment
         *
         * Presumes '/' character is unique, not composite in any MBCS encoding
         */
        if (slash) {
            strendseg = android_strchr(string, '/');
            if (!strendseg)
                strendseg = android_strchr(string, '\0');
        }
        else {
            strendseg = android_strchr(string, '\0');
        }
        /* Allow pattern '*' to be consumed even with no remaining string to match
         */
        while (*pattern)
        {
            if ((string > strendseg)
                || ((string == strendseg) && (*pattern != '*')))
                break;
            if (slash && ((*pattern == '/')
                           || (escape && (*pattern == '\\')
                                      && (pattern[1] == '/'))))
                break;
            /* Reduce groups of '*' and '?' to n '?' matches
             * followed by one '*' test for simplicity
             */
            for (wild = 0; ((*pattern == '*') || (*pattern == '?')); ++pattern)
            {
                if (*pattern == '*') {
                    wild = 1;
                }
                else if (string < strendseg) {  /* && (*pattern == '?') */
                    /* XXX: Advance 1 char for MBCS locale */
                    ++string;
                }
                else {  /* (string >= strendseg) && (*pattern == '?') */
                    return ANDROID_FNM_NOMATCH;
                }
            }
            if (wild)
            {
                strstartseg = string;
                mismatch = pattern;
                /* Count fixed (non '*') char matches remaining in pattern
                 * excluding '/' (or "\/") and '*'
                 */
                for (matchptr = pattern, matchlen = 0; 1; ++matchlen)
                {
                    if ((*matchptr == '\0') 
                        || (slash && ((*matchptr == '/')
                                      || (escape && (*matchptr == '\\')
                                                 && (matchptr[1] == '/')))))
                    {
                        /* Compare precisely this many trailing string chars,
                         * the resulting match needs no wildcard loop
                         */
                        /* XXX: Adjust for MBCS */
                        if (string + matchlen > strendseg)
                            return ANDROID_FNM_NOMATCH;
                        string = strendseg - matchlen;
                        wild = 0;
                        break;
                    }
                    if (*matchptr == '*')
                    {
                        /* Ensure at least this many trailing string chars remain
                         * for the first comparison
                         */
                        /* XXX: Adjust for MBCS */
                        if (string + matchlen > strendseg)
                            return ANDROID_FNM_NOMATCH;
                        /* Begin first wild comparison at the current position */
                        break;
                    }
                    /* Skip forward in pattern by a single character match
                     * Use a dummy android_fnmatch_ch() test to count one "[range]" escape
                     */ 
                    /* XXX: Adjust for MBCS */
                    if (escape && (*matchptr == '\\') && matchptr[1]) {
                        matchptr += 2;
                    }
                    else if (*matchptr == '[') {
                        dummyptr = dummystring;
                        android_fnmatch_ch(&matchptr, &dummyptr, flags);
                    }
                    else {
                        ++matchptr;
                    }
                }
            }
            /* Incrementally match string against the pattern
             */
            while (*pattern && (string < strendseg))
            {
                /* Success; begin a new wild pattern search
                 */
                if (*pattern == '*')
                    break;
                if (slash && ((*string == '/')
                              || (*pattern == '/')
                              || (escape && (*pattern == '\\')
                                         && (pattern[1] == '/'))))
                    break;
                /* Compare ch's (the pattern is advanced over "\/" to the '/',
                 * but slashes will mismatch, and are not consumed)
                 */
                if (!android_fnmatch_ch(&pattern, &string, flags))
                    continue;
                /* Failed to match, loop against next char offset of string segment 
                 * until not enough string chars remain to match the fixed pattern
                 */
                if (wild) {
                    /* XXX: Advance 1 char for MBCS locale */
                    string = ++strstartseg;
                    if (string + matchlen > strendseg)
                        return ANDROID_FNM_NOMATCH;
                    pattern = mismatch;
                    continue;
                }
                else
                    return ANDROID_FNM_NOMATCH;
            }
        }
        if (*string && !((slash || leading_dir) && (*string == '/')))
            return ANDROID_FNM_NOMATCH;
        if (*pattern && !(slash && ((*pattern == '/')
                                    || (escape && (*pattern == '\\')
                                               && (pattern[1] == '/')))))
            return ANDROID_FNM_NOMATCH;
        if (leading_dir && !*pattern && *string == '/')
            return 0;
    }
    /* Where both pattern and string are at EOS, declare success
     */
    if (!*string && !*pattern)
        return 0;
    /* pattern didn't match to the end of string */
    return ANDROID_FNM_NOMATCH;
}