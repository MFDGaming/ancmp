#ifndef ANCMP_ANDROID_MATH_H
#define ANCMP_ANDROID_MATH_H

#include "abi_fix.h"

float android_atan2f(float x, float y) FLOAT_ABI_FIX;

float android_ceilf(float x) FLOAT_ABI_FIX;

float android_cosf(float x) FLOAT_ABI_FIX;

float android_floorf(float x) FLOAT_ABI_FIX;

float android_fmodf(float x, float y) FLOAT_ABI_FIX;

float android_logf(float x) FLOAT_ABI_FIX;

float android_powf(float x, float y) FLOAT_ABI_FIX;

float android_sinf(float x) FLOAT_ABI_FIX;

float android_sqrtf(float x) FLOAT_ABI_FIX;

float android_atanf(float x) FLOAT_ABI_FIX;

double android_fmod(double x, double y) FLOAT_ABI_FIX;

double android_floor(double x) FLOAT_ABI_FIX;

double android_ceil(double x) FLOAT_ABI_FIX;

double android_sin(double x) FLOAT_ABI_FIX;

double android_cos(double x) FLOAT_ABI_FIX;

double android_sqrt(double x) FLOAT_ABI_FIX;

double android_pow(double x, double y) FLOAT_ABI_FIX;

double android_atan(double x) FLOAT_ABI_FIX;

double android_atan2(double x, double y) FLOAT_ABI_FIX;

double android_modf(double x, double *iptr) FLOAT_ABI_FIX;

float android_modff(float x, float *iptr) FLOAT_ABI_FIX;

double android_ldexp(double x, int exp) FLOAT_ABI_FIX;

float android_ldexpf(float x, int exp) FLOAT_ABI_FIX;

float android_tanf(float x) FLOAT_ABI_FIX;

double android_cosh(double x) FLOAT_ABI_FIX;

double android_tan(double x) FLOAT_ABI_FIX;

double android_tanh(double x) FLOAT_ABI_FIX;

double android_asin(double x) FLOAT_ABI_FIX;

double android_log(double x) FLOAT_ABI_FIX;

double android_sinh(double x) FLOAT_ABI_FIX;

double android_acos(double x) FLOAT_ABI_FIX;

double android_exp(double x) FLOAT_ABI_FIX;

double android_frexp(double x, int *exp) FLOAT_ABI_FIX;

double android_log10(double x) FLOAT_ABI_FIX;

float android_log10f(float x) FLOAT_ABI_FIX;

float android_acosf(float x) FLOAT_ABI_FIX;

float android_expf(float x) FLOAT_ABI_FIX;

float android_asinf(float x) FLOAT_ABI_FIX;

float android_exp2f(float x) FLOAT_ABI_FIX;

double android_exp2(double x) FLOAT_ABI_FIX;

#endif
