#ifndef ANCMP_ANDROID_CXA_H
#define ANCMP_ANDROID_CXA_H

int android_cxa_atexit(void (*func)(void *), void *arg, void *d);

void android_cxa_finalize(void *d);

void android_cxa_pure_virtual();

#endif