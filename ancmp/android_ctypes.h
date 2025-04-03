#ifndef ANCMP_ANDROID_CTYPES_H
#define ANCMP_ANDROID_CTYPES_H

#define ANDROID_CTYPE_NUM_CHARS 256

extern const char *android_ctype;

extern const short *android_tolower_tab;

extern const short *android_toupper_tab;

int android_tolower(int c);

int android_toupper(int c);

int android_isalnum(int c);

int android_isalpha(int c);

int android_iscntrl(int c);

int android_isdigit(int c);

int android_isgraph(int c);

int android_islower(int c);

int android_isprint(int c);

int android_ispunct(int c);

int android_isspace(int c);

int android_isupper(int c);

int android_isxdigit(int c);

int android_isblank(int c);

int android_isascii(int c);

int android_toascii(int c);

#endif
