#ifndef _WIN32
#ifdef __arm__
#define FLOAT_ABI_FIX __attribute__((pcs("aapcs")))
#else
#define FLOAT_ABI_FIX
#endif
#else
#define FLOAT_ABI_FIX
#endif