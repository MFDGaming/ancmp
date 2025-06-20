#ifndef ANCMP_ABI_FIX_H
#define ANCMP_ABI_FIX_H

#include <stdlib.h>

#ifndef _WIN32
#ifdef __arm__
#define FLOAT_ABI_FIX __attribute__((pcs("aapcs")))
#else
#define FLOAT_ABI_FIX
#endif
#else
#define FLOAT_ABI_FIX
#endif

#ifdef _WIN32
#define SYSV_WRAPPER_US "_"
#else
#define SYSV_WRAPPER_US ""
#endif

#if defined(__i386__) || defined(_M_IX86)
#if defined(_MSC_VER)

#define SYSV_WRAPPER_1(FUNC) \
    void __declspec(naked) SYSV_WRAPPER_##FUNC() { \
        __asm push ebx \
        __asm mov ebx, esp \
        __asm sub esp, 4 \
        __asm and esp, 0xFFFFFFF0 \
        __asm add esp, 4 \
        __asm push [ebx+8] \
        __asm call FUNC \
        __asm mov eax, [ebx+8] \
        __asm mov esp, ebx \
        __asm pop ebx \
        __asm retn 4 \
    }

#define SYSV_WRAPPER_2(FUNC) \
    void __declspec(naked) SYSV_WRAPPER_##FUNC() { \
        __asm push ebx \
        __asm mov ebx, esp \
        __asm sub esp, 8 \
        __asm and esp, 0xFFFFFFF0 \
        __asm add esp, 8 \
        __asm push [ebx+12] \
        __asm push [ebx+8] \
        __asm call FUNC \
        __asm mov eax, [ebx+8] \
        __asm mov esp, ebx \
        __asm pop ebx \
        __asm retn 4 \
    }

#define SYSV_WRAPPER_3(FUNC) \
    void __declspec(naked) SYSV_WRAPPER_##FUNC() { \
        __asm push ebx \
        __asm mov ebx, esp \
        __asm sub esp, 12 \
        __asm and esp, 0xFFFFFFF0 \
        __asm add esp, 12 \
        __asm push [ebx+16] \
        __asm push [ebx+12] \
        __asm push [ebx+8] \
        __asm call FUNC \
        __asm mov eax, [ebx+8] \
        __asm mov esp, ebx \
        __asm pop ebx \
        __asm retn 4 \
    }

#define SYSV_WRAPPER_4(FUNC) \
    void __declspec(naked) SYSV_WRAPPER_##FUNC() { \
        __asm push ebx \
        __asm mov ebx, esp \
        __asm sub esp, 16 \
        __asm and esp, 0xFFFFFFF0 \
        __asm add esp, 16 \
        __asm push [ebx+20] \
        __asm push [ebx+16] \
        __asm push [ebx+12] \
        __asm push [ebx+8] \
        __asm call FUNC \
        __asm mov eax, [ebx+8] \
        __asm mov esp, ebx \
        __asm pop ebx \
        __asm retn 4 \
    }

#define SYSV_WRAPPER_5(FUNC) \
    void __declspec(naked) SYSV_WRAPPER_##FUNC() { \
        __asm push ebx \
        __asm mov ebx, esp \
        __asm sub esp, 20 \
        __asm and esp, 0xFFFFFFF0 \
        __asm add esp, 20 \
        __asm push [ebx+24] \
        __asm push [ebx+20] \
        __asm push [ebx+16] \
        __asm push [ebx+12] \
        __asm push [ebx+8] \
        __asm call FUNC \
        __asm mov eax, [ebx+8] \
        __asm mov esp, ebx \
        __asm pop ebx \
        __asm retn 4 \
    }

#define SYSV_WRAPPER_6(FUNC) \
    void __declspec(naked) SYSV_WRAPPER_##FUNC() { \
        __asm push ebx \
        __asm mov ebx, esp \
        __asm sub esp, 24 \
        __asm and esp, 0xFFFFFFF0 \
        __asm add esp, 24 \
        __asm push [ebx+28] \
        __asm push [ebx+24] \
        __asm push [ebx+20] \
        __asm push [ebx+16] \
        __asm push [ebx+12] \
        __asm push [ebx+8] \
        __asm call FUNC \
        __asm mov eax, [ebx+8] \
        __asm mov esp, ebx \
        __asm pop ebx \
        __asm retn 4 \
    }

#else

#define SYSV_WRAPPER_1(FUNC) \
    void __attribute__((naked)) SYSV_WRAPPER_##FUNC() { \
        __asm__( \
            "push %ebx \n" \
            "mov %esp, %ebx \n" \
            "sub $4, %esp \n" \
            "and $0xFFFFFFF0, %esp \n" \
            "add $4, %esp \n" \
            "push 8(%ebx) \n" \
            "call " SYSV_WRAPPER_US #FUNC " \n" \
            "mov 8(%ebx), %eax \n" \
            "mov %ebx, %esp \n" \
            "pop %ebx \n" \
            "ret $4 \n" \
        ); \
    }

#define SYSV_WRAPPER_2(FUNC) \
     void __attribute__((naked)) SYSV_WRAPPER_##FUNC() { \
        __asm__( \
            "push %ebx \n" \
            "mov %esp, %ebx \n" \
            "sub $8, %esp \n" \
            "and $0xFFFFFFF0, %esp \n" \
            "add $8, %esp \n" \
            "push 12(%ebx) \n" \
            "push 8(%ebx) \n" \
            "call " SYSV_WRAPPER_US #FUNC " \n" \
            "mov 8(%ebx), %eax \n" \
            "mov %ebx, %esp \n" \
            "pop %ebx \n" \
            "ret $4 \n" \
        ); \
    }

#define SYSV_WRAPPER_3(FUNC) \
    void __attribute__((naked)) SYSV_WRAPPER_##FUNC() { \
        __asm__( \
            "push %ebx \n" \
            "mov %esp, %ebx \n" \
            "sub $12, %esp \n" \
            "and $0xFFFFFFF0, %esp \n" \
            "add $12, %esp \n" \
            "push 16(%ebx) \n" \
            "push 12(%ebx) \n" \
            "push 8(%ebx) \n" \
            "call " SYSV_WRAPPER_US #FUNC " \n" \
            "mov 8(%ebx), %eax \n" \
            "mov %ebx, %esp \n" \
            "pop %ebx \n" \
            "ret $4 \n" \
        ); \
    }

#define SYSV_WRAPPER_4(FUNC) \
    void __attribute__((naked)) SYSV_WRAPPER_##FUNC() { \
        __asm__( \
            "push %ebx \n" \
            "mov %esp, %ebx \n" \
            "sub $16, %esp \n" \
            "and $0xFFFFFFF0, %esp \n" \
            "add $16, %esp \n" \
            "push 20(%ebx) \n" \
            "push 16(%ebx) \n" \
            "push 12(%ebx) \n" \
            "push 8(%ebx) \n" \
            "call " SYSV_WRAPPER_US #FUNC " \n" \
            "mov 8(%ebx), %eax \n" \
            "mov %ebx, %esp \n" \
            "pop %ebx \n" \
            "ret $4 \n" \
        ); \
    }

#define SYSV_WRAPPER_5(FUNC) \
    void __attribute__((naked)) SYSV_WRAPPER_##FUNC() { \
        __asm__( \
            "push %ebx \n" \
            "mov %esp, %ebx \n" \
            "sub $20, %esp \n" \
            "and $0xFFFFFFF0, %esp \n" \
            "add $20, %esp \n" \
            "push 24(%ebx) \n" \
            "push 20(%ebx) \n" \
            "push 16(%ebx) \n" \
            "push 12(%ebx) \n" \
            "push 8(%ebx) \n" \
            "call " SYSV_WRAPPER_US #FUNC " \n" \
            "mov 8(%ebx), %eax \n" \
            "mov %ebx, %esp \n" \
            "pop %ebx \n" \
            "ret $4 \n" \
        ); \
    }

#define SYSV_WRAPPER_6(FUNC) \
    void __attribute__((naked)) SYSV_WRAPPER_##FUNC() { \
        __asm__( \
            "push %ebx \n" \
            "mov %esp, %ebx \n" \
            "sub $24, %esp \n" \
            "and $0xFFFFFFF0, %esp \n" \
            "add $24, %esp \n" \
            "push 28(%ebx) \n" \
            "push 24(%ebx) \n" \
            "push 20(%ebx) \n" \
            "push 16(%ebx) \n" \
            "push 12(%ebx) \n" \
            "push 8(%ebx) \n" \
            "call " SYSV_WRAPPER_US #FUNC " \n" \
            "mov 8(%ebx), %eax \n" \
            "mov %ebx, %esp \n" \
            "pop %ebx \n" \
            "ret $4 \n" \
        ); \
    }

#endif

#define SYSV_WRAPPER(FUNC, ARG_CNT) SYSV_WRAPPER_##ARG_CNT(FUNC)

#define GET_SYSV_WRAPPER(FUNC) SYSV_WRAPPER_##FUNC

#define EXTERN_SYSV_WRAPPER(FUNC) extern void SYSV_WRAPPER_##FUNC();

void sysv_call_func(void *func, void *retval, int argc, ...);

void call_with_custom_stack(void *func, int *retval, size_t stack_size, int argc, ...);

#else
#define SYSV_WRAPPER(FUNC, ARG_CNT)

#define GET_SYSV_WRAPPER(FUNC) FUNC

#define EXTERN_SYSV_WRAPPER(FUNC)
#endif

#endif
