#ifndef ANCMP_STDINT
#define ANCMP_STDINT

#ifndef ANCMP_NO_STDINT
#include <stdint.h>
#else

#ifdef _WIN32
typedef signed __int8 int8_t;
typedef signed __int16 int16_t;
typedef signed __int32 int32_t;
typedef signed __int64 int64_t;

typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
#endif

typedef int8_t int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;

typedef uint8_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;

typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef signed long intptr_t;
typedef unsigned long uintptr_t;

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

#define INT8_MIN (-128)
#define INT8_MAX (127)
#define UINT8_MAX (255)

#define INT16_MIN (-32768)
#define INT16_MAX (32767)
#define UINT16_MAX (65535)

#define INT32_MIN (-2147483648)
#define INT32_MAX (2147483647)
#define UINT32_MAX (4294967295U)

#define INT64_MIN (-(int64_t)9223372036854775807 - 1)
#define INT64_MAX ((int64_t)9223372036854775807)
#define UINT64_MAX ((uint64_t)18446744073709551615)

#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#endif

#endif