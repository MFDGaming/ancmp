#include "abi_fix.h"
#include <stdarg.h>
#include <stdlib.h>

#if defined(__i386__) || defined(_M_IX86)
void sysv_call_func(void *func, void *ret, int argc, ...) {
    int i;
    va_list ap;
    void **args = NULL;
    if (argc) {
        args = (void **)malloc(argc * sizeof(void *));
        if (!args) {
            return;
        }
        va_start(ap, argc);
        for (i = 0; i < argc; ++i) {
            args[(argc - 1) - i] = va_arg(ap, void *);
        }
        va_end(ap);
    }
#if defined(_MSC_VER)
    __asm {
        push ebx
        mov ebx, esp
        and esp, 0xFFFFFFF0
        cmp argc, 0
        je sysv_wrapper_skip_null
        mov eax, args
        sub eax, 4
    sysv_wrapper_start:
        add eax, 4
        push [eax]
        cmp eax, args
        jne sysv_wrapper_start
    sysv_wrapper_skip_null:
        push ret
        mov eax, func
        call eax
        mov esp, ebx
        pop ebx
    }
#else
    asm volatile (
        "push %%ebx\n"
        "mov %%esp, %%ebx\n"
        "and $0xFFFFFFF0, %%esp\n"
        "cmp $0, %0\n"
        "je sysv_wrapper_skip_null\n"
        "mov %1, %%eax\n"
        "sub $4, %%eax\n"
        "sysv_wrapper_start:\n"
        "add $4, %%eax\n"
        "push (%%eax)\n"
        "cmp %1, %%eax\n"
        "jne sysv_wrapper_start\n"
        "sysv_wrapper_skip_null:\n"
        "push %3\n"
        "call *%2\n"
        "mov %%ebx, %%esp\n"
        "pop %%ebx\n"
        :
        : "r"(argc), "r"(args), "r"(func), "r"(ret)
        : "eax", "ebx"
    );
#endif
}
#endif
