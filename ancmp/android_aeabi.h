#ifndef ANCMP_ANDROID_AEABI
#define ANCMP_ANDROID_AEABI

#include "ancmp_stdint.h"
#include "android_linker_defs.h"
#include "abi_fix.h"

int android_aeabi_atexit(void *object, void (*destructor)(void *), void *dso_handle);

void android_aeabi_memcpy8(void *dest, const void *src, size_t n);

void android_aeabi_memcpy4(void *dest, const void *src, size_t n);

void android_aeabi_memcpy(void *dest, const void *src, size_t n);

void android_aeabi_memmove8(void *dest, const void *src, size_t n);

void android_aeabi_memmove4(void *dest, const void *src, size_t n);

void android_aeabi_memmove(void *dest, const void *src, size_t n);

void android_aeabi_memset8(void *dest, size_t n, int c);

void android_aeabi_memset4(void *dest, size_t n, int c);

void android_aeabi_memset(void *dest, size_t n, int c);

void android_aeabi_memclr8(void *dest, size_t n);

void android_aeabi_memclr4(void *dest, size_t n);

void android_aeabi_memclr(void *dest, size_t n);

float android_aeabi_ul2f(uint64_t v) FLOAT_ABI_FIX;

double android_aeabi_ul2d(uint64_t v) FLOAT_ABI_FIX;

uint64_t android_aeabi_d2ulz(double v) FLOAT_ABI_FIX;

int32_t android_aeabi_idiv(int32_t numerator, int32_t denominator);

uint32_t android_aeabi_uidiv(uint32_t numerator, uint32_t denominator);

int64_t android_aeabi_ldiv(int64_t numerator, int64_t denominator);

uint64_t android_aeabi_uldiv(uint64_t numerator, uint64_t denominator);

#if !defined(_MSC_VER) && ANDROID_ARM_LINKER

void android_aeabi_idivmod(int32_t numerator, int32_t denominator);

void android_aeabi_uidivmod(uint32_t numerator, uint32_t denominator);

void android_aeabi_ldivmod(int64_t numerator, int64_t denominator);

void android_aeabi_uldivmod(uint64_t numerator, uint64_t denominator);

#endif

#endif