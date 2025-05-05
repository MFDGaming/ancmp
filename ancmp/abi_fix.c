#include "abi_fix.h"
#include <stdarg.h>
#include <stdlib.h>

#if defined(__i386__) || defined(_M_IX86)
void sysv_call_func(void *func, void *ret, int argc, ...) {
    int i;
    va_list ap;
    void **args = (void **)malloc(argc * sizeof(void *));
    if (!args) {
        return;
    }
    va_start(ap, argc);
    for (i = 0; i < argc; ++i) {
        args[(argc - 1) - i] = va_arg(ap, void *);
    }
#if defined(_MSC_VER)
    __asm {
        mov eax, args
        sub eax, 4
    sysv_wrapper_start:
        add eax, 4
        push [eax]
        cmp eax, args
        jne sysv_wrapper_start
        push ret
        mov eax, func
        call eax
        mov eax, argc
        shl eax, 2
        add esp, eax
    }
#else
    va_end(ap);
    asm volatile (
        "mov %1, %%eax\n"
        "sub $4, %%eax\n"
        "sysv_wrapper_start:\n"
        "add $4, %%eax\n"
        "push (%%eax)\n"
        "cmp %1, %%eax\n"
        "jne sysv_wrapper_start\n"
        "push %3\n"
        "call *%2\n"
        "mov %0, %%eax\n"
        "shl $2, %%eax\n"
        "add %%eax, %%esp\n"
        :
        : "r"(argc), "r"(args), "r"(func), "r"(ret)
        : "eax"
    );
#endif
}
#endif
