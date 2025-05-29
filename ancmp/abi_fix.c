#include "abi_fix.h"
#include <stdarg.h>
#include <stdlib.h>

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
        push ebx
        mov ebx, esp
        and esp, 0xFFFFFFF0
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
    }
#else
    asm volatile (
        "push %%ebx \n"
        "mov %%esp, %%ebx \n"
        "and $0xFFFFFFF0, %%esp \n"
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
        :
        : [args]"r"(args), [args_end]"r"(args_end), [func]"r"(func), [retval]"r"(retval)
        : "eax", "ebx", "memory"
    );
#endif
    if (args) {
        free(args);
    }
}

#ifdef _WIN32
#include <windows.h>

void call_with_custom_stack(void *func, int *retval, size_t stack_size, int argc, ...) {
    int i;
    va_list ap;
    void **args = NULL;
    void **args_end = NULL;
    char *stack = NULL, *stack_top = NULL;
    int rv = 0;
    
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

    stack = (char *)VirtualAlloc(NULL, stack_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!stack) {
        if (args) {
            free(args);
        }
        return;
    }
    stack_top = stack + (stack_size - 1);
    *(int *)&stack_top &= 0xfffffff0;

#if defined(_MSC_VER)
    __asm {
        push ebx
        mov ebx, esp
        mov esp, stack_top
        mov eax, args
        cmp eax, 0
        je custom_stack_loop_end
        custom_stack_loop_start:
        push [eax]
        add eax, 4
        cmp eax, args_end
        jne custom_stack_loop_start
        custom_stack_loop_end:
        call func
        mov esp, ebx
        pop ebx
        mov rv, eax
    }
#else
    asm volatile (
        "push %%ebx \n"
        "mov %%esp, %%ebx \n"
        "mov %[stack_top], %%esp \n"
        "mov %[args], %%eax \n"
        "cmp $0, %%eax \n"
        "je custom_stack_loop_end \n"
        "custom_stack_loop_start: \n"
        "push (%%eax) \n"
        "add $4, %%eax \n"
        "cmp %[args_end], %%eax \n"
        "jne custom_stack_loop_start \n"
        "custom_stack_loop_end: \n"
        "call *%[func] \n"
        "mov %%ebx, %%esp \n"
        "pop %%ebx \n"
        "mov %%eax, %[rv]"
        : [rv]"=r"(rv)
        : [args]"r"(args), [args_end]"r"(args_end), [func]"r"(func), [stack_top]"r"(stack_top)
        : "eax", "ebx", "memory"
    );
#endif
    VirtualFree(stack, 0, MEM_RELEASE);
    if (retval) {
        *retval = rv;
    }
    if (args) {
        free(args);
    }
}
#endif

#endif
