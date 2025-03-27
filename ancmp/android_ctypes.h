#pragma once

#define ANDROID_CTYPE_NUM_CHARS 256

extern const char *android_ctype;

extern const short *android_tolower_tab;

extern const short *android_toupper_tab;

int android_tolower(int c);

int android_toupper(int c);