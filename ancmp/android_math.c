#include "android_math.h"
#include <math.h>

FLOAT_ABI_FIX float android_atan2f(float x, float y) {
    return atan2f(x, y);
}

FLOAT_ABI_FIX float android_ceilf(float x) {
    return ceilf(x);
}

FLOAT_ABI_FIX float android_cosf(float x) {
    return cosf(x);
}

FLOAT_ABI_FIX float android_floorf(float x) {
    return floorf(x);
}

FLOAT_ABI_FIX float android_fmodf(float x, float y) {
    return fmodf(x, y);
}

FLOAT_ABI_FIX float android_logf(float x) {
    return logf(x);
}

FLOAT_ABI_FIX float android_powf(float x, float y) {
    return powf(x, y);
}

FLOAT_ABI_FIX float android_sinf(float x) {
    return sinf(x);
}

FLOAT_ABI_FIX float android_sqrtf(float x) {
    return sqrtf(x);
}

FLOAT_ABI_FIX float android_atanf(float x) {
    return atanf(x);
}

FLOAT_ABI_FIX double android_fmod(double x, double y) {
    return fmod(x, y);
}

FLOAT_ABI_FIX double android_floor(double x) {
    return floor(x);
}

FLOAT_ABI_FIX double android_ceil(double x) {
    return ceil(x);
}

FLOAT_ABI_FIX double android_sin(double x) {
    return sin(x);
}

FLOAT_ABI_FIX double android_cos(double x) {
    return cos(x);
}

FLOAT_ABI_FIX double android_sqrt(double x) {
    return sqrt(x);
}

FLOAT_ABI_FIX double android_pow(double x, double y) {
    return pow(x, y);
}

FLOAT_ABI_FIX double android_atan(double x) {
    return atan(x);
}

FLOAT_ABI_FIX double android_atan2(double x, double y) {
    return atan2(x, y);
}

FLOAT_ABI_FIX double android_modf(double x, double *iptr) {
    return modf(x, iptr);
}

FLOAT_ABI_FIX float android_modff(float x, float *iptr) {
    return modff(x, iptr);
}

FLOAT_ABI_FIX double android_ldexp(double x, int exp) {
    return ldexp(x, exp);
}

FLOAT_ABI_FIX float android_ldexpf(float x, int exp) {
    return ldexpf(x, exp);
}

FLOAT_ABI_FIX float android_tanf(float x) {
    return tanf(x);
}