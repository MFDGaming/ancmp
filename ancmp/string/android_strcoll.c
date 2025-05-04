#include "android_strcoll.h"
#include "android_strcmp.h"

int android_strcoll(const char *s1, const char *s2) {
	return android_strcmp(s1, s2);
}
