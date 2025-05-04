#include "android_aeabi.h"
#include "android_cxa.h"
#include "string/android_string.h"

int android_aeabi_atexit(void *object, void (*destructor)(void *), void *dso_handle) {
    return android_cxa_atexit(destructor, object, dso_handle);
}

void android_aeabi_memcpy8(void *dest, const void *src, size_t n) {
    android_memcpy(dest, src, n);
}

void android_aeabi_memcpy4(void *dest, const void *src, size_t n) {
    android_memcpy(dest, src, n);
}

void android_aeabi_memcpy(void *dest, const void *src, size_t n) {
    android_memcpy(dest, src, n);
}

void android_aeabi_memmove8(void *dest, const void *src, size_t n) {
    android_memmove(dest, src, n);
}

void android_aeabi_memmove4(void *dest, const void *src, size_t n) {
    android_memmove(dest, src, n);
}

void android_aeabi_memmove(void *dest, const void *src, size_t n) {
    android_memmove(dest, src, n);
}

void android_aeabi_memset8(void *dest, size_t n, int c) {
    android_memset(dest, c, n);
}

void android_aeabi_memset4(void *dest, size_t n, int c) {
    android_memset(dest, c, n);
}

void android_aeabi_memset(void *dest, size_t n, int c) {
    android_memset(dest, c, n);
}

void android_aeabi_memclr8(void *dest, size_t n) {
    android_aeabi_memset8(dest, n, 0);
}

void android_aeabi_memclr4(void *dest, size_t n) {
    android_aeabi_memset4(dest, n, 0);
}

void android_aeabi_memclr(void *dest, size_t n) {
    android_aeabi_memset(dest, n, 0);
}

FLOAT_ABI_FIX float android_aeabi_ul2f(uint64_t v) {
    return (float)v;
}

FLOAT_ABI_FIX double android_aeabi_ul2d(uint64_t v) {
    return (double)v;
}

FLOAT_ABI_FIX uint64_t android_aeabi_d2ulz(double v) {
    return (uint64_t)v;
}

int32_t android_aeabi_idiv(int32_t numerator, int32_t denominator) {
    return numerator / denominator;
}

uint32_t android_aeabi_uidiv(uint32_t numerator, uint32_t denominator) {
    return numerator / denominator;
}

int64_t android_aeabi_ldiv(int64_t numerator, int64_t denominator) {
    return numerator / denominator;
}

uint64_t android_aeabi_uldiv(uint64_t numerator, uint64_t denominator) {
    return numerator / denominator;
}

#if !defined(_MSC_VER) && defined(ANDROID_ARM_LINKER)

void android_aeabi_idivmod(int32_t numerator, int32_t denominator) {
    int32_t quot = numerator / denominator;
    int32_t rem = numerator % denominator;
    __asm__ (
        "mov r0, %0\n"
        "mov r1, %1\n"
        : 
        : "r" (quot), "r" (rem)
        : "r0", "r1"
    );
}

void android_aeabi_uidivmod(uint32_t numerator, uint32_t denominator) {
    uint32_t quot = numerator / denominator;
    uint32_t rem = numerator % denominator;
    __asm__ (
        "mov r0, %0\n"
        "mov r1, %1\n"
        : 
        : "r" (quot), "r" (rem)
        : "r0", "r1"
    );
}

void android_aeabi_ldivmod(int64_t numerator, int64_t denominator) {
    int64_t quot = numerator / denominator;
    int64_t rem = numerator % denominator;

    __asm__ (
        "mov r0, %0\n"
        "mov r1, %1\n"
        "mov r2, %2\n"
        "mov r3, %3\n"
        :
        : "r" (quot), "r" ((int32_t)(quot >> 32)), "r" (rem), "r" ((int32_t)(rem >> 32))
        : "r0", "r1", "r2", "r3"
    );
}

void android_aeabi_uldivmod(uint64_t numerator, uint64_t denominator) {
    uint64_t quot = numerator / denominator;
    uint64_t rem = numerator % denominator;

    __asm__ (
        "mov r0, %0\n"
        "mov r1, %1\n"
        "mov r2, %2\n"
        "mov r3, %3\n"
        :
        : "r" (quot), "r" ((int32_t)(quot >> 32)), "r" (rem), "r" ((int32_t)(rem >> 32))
        : "r0", "r1", "r2", "r3"
    );
}

#endif
