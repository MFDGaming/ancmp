#ifndef ANCMP_ANDROID_FNMATCH
#define ANCMP_ANDROID_FNMATCH

#define ANDROID_FNM_NOMATCH 1
#define ANDROID_FNM_NOSYS 2
#define ANDROID_FNM_NOESCAPE 0x01
#define ANDROID_FNM_PATHNAME 0x02
#define ANDROID_FNM_PERIOD 0x04
#define ANDROID_FNM_LEADING_DIR 0x08
#define ANDROID_FNM_CASEFOLD 0x10
#define ANDROID_FNM_IGNORECASE ANDROID_FNM_CASEFOLD
#define ANDROID_FNM_FILE_NAME ANDROID_FNM_PATHNAME
#define	ANDROID_RANGE_MATCH	1
#define	ANDROID_RANGE_NOMATCH 0
#define	ANDROID_RANGE_ERROR	(-1)

typedef struct {
	const char *name;
	int (*isctype)(int);
} android_cclass_t;

int android_fnmatch(const char *pattern, const char *string, int flags);

#endif
