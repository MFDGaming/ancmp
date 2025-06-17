#include "abi_fix.h"
#include <stdarg.h>
#include <stdlib.h>
#include "ancmp_stdint.h"

#if defined(__i386__) || defined(_M_IX86)

void sysv_call_func(void *func, void *retval, int argc, ...) {
    int i;
    va_list ap;
    void **args = NULL;
    void **args_end = NULL;
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
        args_end = &args[argc];
    }
#if defined(_MSC_VER)
    __asm {
        push eax
        push ecx
        push edx
        push ebx
        mov ebx, esp
        mov eax, args_end
        sub eax, args
        add eax, 4
        sub esp, eax
        and esp, 0xFFFFFFF0
        add esp, eax
        mov eax, args
        cmp eax, 0
        je sysv_wrapper_loop_end
        sysv_wrapper_loop_start:
        push [eax]
        add eax, 4
        cmp eax, args_end
        jne sysv_wrapper_loop_start
        sysv_wrapper_loop_end:
        push retval
        call func
        mov esp, ebx
        pop ebx
        pop edx
        pop ecx
        pop eax
    }
#else
    asm volatile (
        "push %%eax \n"
        "push %%ecx \n"
        "push %%edx \n"
        "push %%ebx \n"
        "mov %%esp, %%ebx \n"
        "mov %[args_end], %%eax \n"
        "sub %[args], %%eax \n"
        "add $4, %%eax \n"
        "sub %%eax, %%esp \n"
        "and $0xFFFFFFF0, %%esp \n"
        "add %%eax, %%esp \n"
        "mov %[args], %%eax \n"
        "cmp $0, %%eax \n"
        "je sysv_wrapper_loop_end \n"
        "sysv_wrapper_loop_start: \n"
        "push (%%eax) \n"
        "add $4, %%eax \n"
        "cmp %[args_end], %%eax \n"
        "jne sysv_wrapper_loop_start \n"
        "sysv_wrapper_loop_end: \n"
        "push %[retval] \n"
        "call *%[func] \n"
        "mov %%ebx, %%esp \n"
        "pop %%ebx \n"
        "pop %%edx \n"
        "pop %%ecx \n"
        "pop %%eax \n"
        :
        : [args]"r"(args), [args_end]"r"(args_end), [func]"r"(func), [retval]"r"(retval)
        : "eax", "ebx", "memory"
    );
#endif
    if (args) {
        free(args);
    }
}

void call_with_custom_stack(void *func, int *retval, size_t stack_size, int argc, ...) {
    int i;
    va_list ap;
    unsigned long *stack = NULL, *stack_top = NULL;
    int rv = 0;

    stack = (unsigned long *)calloc(stack_size, 1);
    if (!stack) {
        return;
    }
    stack_top = (unsigned long *)(((uintptr_t)stack + (stack_size - 1)) & ~0x0f);
    
    if (argc) {
        stack_top -= argc;
        stack_top = (unsigned long *)((uintptr_t)stack_top & ~0x0f);

        va_start(ap, argc);
        for (i = 0; i < argc; ++i) {
            stack_top[i] = (unsigned long)va_arg(ap, void *);
        }
        va_end(ap);
    }

#if defined(_MSC_VER)
    __asm {
        push eax
        push ecx
        push edx
        push ebx
        mov ebx, esp
        mov esp, stack_top
        call func
        mov rv, eax
        mov esp, ebx
        pop ebx
        pop edx
        pop ecx
        pop eax
    }
#else
    asm volatile (
        "push %%eax \n"
        "push %%ecx \n"
        "push %%edx \n"
        "push %%ebx \n"
        "mov %%esp, %%ebx \n"
        "mov %[stack_top], %%esp \n"
        "call *%[func] \n"
        "mov %%eax, %[rv] \n"
        "mov %%ebx, %%esp \n"
        "pop %%ebx \n"
        "pop %%edx \n"
        "pop %%ecx \n"
        "pop %%eax \n"
        : [rv]"=r"(rv)
        : [func]"r"(func), [stack_top]"r"(stack_top)
        : "eax", "ebx", "memory"
    );
#endif
    if (stack) {
        free(stack);
    }
    if (retval) {
        *retval = rv;
    }
}

#endif
