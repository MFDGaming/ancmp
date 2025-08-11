#include "android_math.h"
#include <math.h>

FLOAT_ABI_FIX float android_atan2f(float x, float y) {
    return (float)atan2((double)x, (double)y);
}

FLOAT_ABI_FIX float android_ceilf(float x) {
    return (float)ceil((double)x);
}

FLOAT_ABI_FIX float android_cosf(float x) {
    return (float)cos((double)x);
}

FLOAT_ABI_FIX float android_floorf(float x) {
    return (float)floor((double)x);
}

FLOAT_ABI_FIX float android_fmodf(float x, float y) {
    return (float)fmod((double)x, (double)y);
}

FLOAT_ABI_FIX float android_logf(float x) {
    return (float)log((double)x);
}

FLOAT_ABI_FIX float android_powf(float x, float y) {
    return (float)pow((double)x, (double)y);
}

FLOAT_ABI_FIX float android_sinf(float x) {
    return (float)sin((double)x);
}

FLOAT_ABI_FIX float android_sqrtf(float x) {
    return (float)sqrt((double)x);
}

FLOAT_ABI_FIX float android_atanf(float x) {
    return (float)atan((double)x);
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
    double i;
    float ret = (float)modf((double)x, &i);
    *iptr = (float)i;
    return ret;
}

FLOAT_ABI_FIX double android_ldexp(double x, int exp) {
    return ldexp(x, exp);
}

FLOAT_ABI_FIX float android_ldexpf(float x, int exp) {
    return (float)ldexp((double)x, exp);
}

FLOAT_ABI_FIX float android_tanf(float x) {
    return (float)tan((double)x);
}

FLOAT_ABI_FIX double android_cosh(double x) {
    return cosh(x);
}

FLOAT_ABI_FIX double android_tan(double x) {
    return tan(x);
}

FLOAT_ABI_FIX double android_tanh(double x) {
    return tanh(x);
}

FLOAT_ABI_FIX double android_asin(double x) {
    return asin(x);
}

FLOAT_ABI_FIX double android_log(double x) {
    return log(x);
}

FLOAT_ABI_FIX double android_sinh(double x) {
    return sinh(x);
}

FLOAT_ABI_FIX double android_acos(double x) {
    return acos(x);
}

FLOAT_ABI_FIX double android_exp(double x) {
    return exp(x);
}

FLOAT_ABI_FIX double android_frexp(double x, int *exp) {
    return frexp(x, exp);
}

FLOAT_ABI_FIX double android_log10(double x) {
    return log10(x);
}

FLOAT_ABI_FIX float android_log10f(float x) {
    return (float)log10((double)x);
}
