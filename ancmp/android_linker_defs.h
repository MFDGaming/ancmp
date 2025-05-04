#ifndef ANCMP_ANDROID_LINKER_DEFS
#define ANCMP_ANDROID_LINKER_DEFS

#if defined(__i386__) || defined(_M_IX86)
#define ANDROID_X86_LINKER
#else
#if defined(__arm__) || defined(_M_ARM) 
#define ANDROID_ARM_LINKER
#else
#error Unsupported architecture. Only ARM and x86 are presently supported.
#endif
#endif

#endif
