/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "android_dlfcn.h"
#include "android_pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linker.h"
#include "linker_format.h"
#include "android_elf.h"

#if defined(_MSC_VER)
    #include <intrin.h>
    #define GET_RETURN_ADDRESS() _ReturnAddress()
#else
    #define GET_RETURN_ADDRESS() __builtin_return_address(0)
#endif

/* This file hijacks the symbols stubbed out in libdl.so. */

#define ANDROID_DL_SUCCESS                    0
#define ANDROID_DL_ERR_CANNOT_LOAD_LIBRARY    1
#define ANDROID_DL_ERR_INVALID_LIBRARY_HANDLE 2
#define ANDROID_DL_ERR_BAD_SYMBOL_NAME        3
#define ANDROID_DL_ERR_SYMBOL_NOT_FOUND       4
#define ANDROID_DL_ERR_SYMBOL_NOT_GLOBAL      5

static char dl_err_buf[1024];
static const char *dl_err_str;

static const char *dl_errors[] = {
    [ANDROID_DL_ERR_CANNOT_LOAD_LIBRARY] = "Cannot load library",
    [ANDROID_DL_ERR_INVALID_LIBRARY_HANDLE] = "Invalid library handle",
    [ANDROID_DL_ERR_BAD_SYMBOL_NAME] = "Invalid symbol name",
    [ANDROID_DL_ERR_SYMBOL_NOT_FOUND] = "Symbol not found",
    [ANDROID_DL_ERR_SYMBOL_NOT_GLOBAL] = "Symbol is not global",
};

android_pthread_mutex_t dl_lock = ANDROID_PTHREAD_MUTEX_INITIALIZER;

static void set_dlerror(int err)
{
    format_buffer(dl_err_buf, sizeof(dl_err_buf), "%s: %s", dl_errors[err],
             linker_get_error());
    dl_err_str = (const char *)&dl_err_buf[0];
};

void *android_dlopen(const char *filename, int flag)
{
    soinfo *ret;

    android_pthread_mutex_lock(&dl_lock);
    ret = find_library(filename);
    if (ret == NULL) {
        set_dlerror(ANDROID_DL_ERR_CANNOT_LOAD_LIBRARY);
    } else {
        call_constructors_recursive(ret);
        ret->refcount++;
    }
    android_pthread_mutex_unlock(&dl_lock);
    return ret;
}

const char *android_dlerror(void)
{
    const char *tmp = dl_err_str;
    dl_err_str = NULL;
    return (const char *)tmp;
}

void *android_dlsym(void *handle, const char *symbol)
{
    soinfo *found;
    Elf32_Sym *sym;
    unsigned bind;

    android_pthread_mutex_lock(&dl_lock);

    if(handle == 0) { 
        set_dlerror(ANDROID_DL_ERR_INVALID_LIBRARY_HANDLE);
        goto err;
    }
    if(symbol == 0) {
        set_dlerror(ANDROID_DL_ERR_BAD_SYMBOL_NAME);
        goto err;
    }

    if(handle == ANDROID_RTLD_DEFAULT) {
        sym = lookup(symbol, &found, NULL);
    } else if(handle == ANDROID_RTLD_NEXT) {
        void *ret_addr = GET_RETURN_ADDRESS();
        soinfo *si = find_containing_library(ret_addr);

        sym = NULL;
        if(si && si->next) {
            sym = lookup(symbol, &found, si->next);
        }
    } else {
        found = (soinfo*)handle;
        sym = lookup_in_library(found, symbol);
    }

    if(sym != 0) {
        bind = ELF32_ST_BIND(sym->st_info);

        if((bind == STB_GLOBAL || bind == STB_WEAK) && (sym->st_shndx != 0)) {
            unsigned ret = sym->st_value + found->base;
            android_pthread_mutex_unlock(&dl_lock);
            return (void*)ret;
        }

        set_dlerror(ANDROID_DL_ERR_SYMBOL_NOT_GLOBAL);
    }
    else
        set_dlerror(ANDROID_DL_ERR_SYMBOL_NOT_FOUND);

err:
    android_pthread_mutex_unlock(&dl_lock);
    return 0;
}

int android_dladdr(const void *addr, android_Dl_info *info)
{
    int ret = 0;

    android_pthread_mutex_lock(&dl_lock);

    /* Determine if this address can be found in any library currently mapped */
    soinfo *si = find_containing_library(addr);

    if(si) {
        memset(info, 0, sizeof(android_Dl_info));

        info->dli_fname = si->name;
        info->dli_fbase = (void*)si->base;

        /* Determine if any symbol in the library contains the specified address */
        Elf32_Sym *sym = find_containing_symbol(addr, si);

        if(sym != NULL) {
            info->dli_sname = si->strtab + sym->st_name;
            info->dli_saddr = (void*)(si->base + sym->st_value);
        }

        ret = 1;
    }

    android_pthread_mutex_unlock(&dl_lock);

    return ret;
}

int android_dlclose(void *handle)
{
    android_pthread_mutex_lock(&dl_lock);
    (void)unload_library((soinfo*)handle);
    android_pthread_mutex_unlock(&dl_lock);
    return 0;
}

#if defined(ANDROID_ARM_LINKER)
//                     0000000 00011111 111112 22222222 2333333 333344444444445555555
//                     0123456 78901234 567890 12345678 9012345 678901234567890123456
#define ANDROID_LIBDL_STRTAB \
                      "dlopen\0dlclose\0dlsym\0dlerror\0dladdr\0dl_unwind_find_exidx\0"

#elif defined(ANDROID_X86_LINKER)
//                     0000000 00011111 111112 22222222 2333333 3333444444444455
//                     0123456 78901234 567890 12345678 9012345 6789012345678901
#define ANDROID_LIBDL_STRTAB \
                      "dlopen\0dlclose\0dlsym\0dlerror\0dladdr\0dl_iterate_phdr\0"
#else
#error Unsupported architecture. Only ARM and x86 are presently supported.
#endif


static Elf32_Sym libdl_symtab[] = {
      // total length of libdl_info.strtab, including trailing 0
      // This is actually the the STH_UNDEF entry. Technically, it's
      // supposed to have st_name == 0, but instead, it points to an index
      // in the strtab with a \0 to make iterating through the symtab easier.
    { .st_name = sizeof(ANDROID_LIBDL_STRTAB) - 1,
    },
    { .st_name = 0,   // starting index of the name in libdl_info.strtab
      .st_value = (Elf32_Addr) &android_dlopen,
      .st_info = STB_GLOBAL << 4,
      .st_shndx = 1,
    },
    { .st_name = 7,
      .st_value = (Elf32_Addr) &android_dlclose,
      .st_info = STB_GLOBAL << 4,
      .st_shndx = 1,
    },
    { .st_name = 15,
      .st_value = (Elf32_Addr) &android_dlsym,
      .st_info = STB_GLOBAL << 4,
      .st_shndx = 1,
    },
    { .st_name = 21,
      .st_value = (Elf32_Addr) &android_dlerror,
      .st_info = STB_GLOBAL << 4,
      .st_shndx = 1,
    },
    { .st_name = 29,
      .st_value = (Elf32_Addr) &android_dladdr,
      .st_info = STB_GLOBAL << 4,
      .st_shndx = 1,
    },
#ifdef ANDROID_ARM_LINKER
    { .st_name = 36,
      .st_value = (Elf32_Addr) &android_dl_unwind_find_exidx,
      .st_info = STB_GLOBAL << 4,
      .st_shndx = 1,
    },
#elif defined(ANDROID_X86_LINKER)
    { .st_name = 36,
      .st_value = (Elf32_Addr) &android_dl_iterate_phdr,
      .st_info = STB_GLOBAL << 4,
      .st_shndx = 1,
    },
#endif
};

/* Fake out a hash table with a single bucket.
 * A search of the hash table will look through
 * libdl_symtab starting with index [1], then
 * use libdl_chains to find the next index to
 * look at.  libdl_chains should be set up to
 * walk through every element in libdl_symtab,
 * and then end with 0 (sentinel value).
 *
 * I.e., libdl_chains should look like
 * { 0, 2, 3, ... N, 0 } where N is the number
 * of actual symbols, or nelems(libdl_symtab)-1
 * (since the first element of libdl_symtab is not
 * a real symbol).
 *
 * (see _elf_lookup())
 *
 * Note that adding any new symbols here requires
 * stubbing them out in libdl.
 */
static unsigned libdl_buckets[1] = { 1 };
static unsigned libdl_chains[7] = { 0, 2, 3, 4, 5, 6, 0 };

soinfo libdl_info = {
    .name = "libdl.so",
    .flags = FLAG_LINKED,

    .strtab = ANDROID_LIBDL_STRTAB,
    .symtab = libdl_symtab,

    .nbucket = 1,
    .nchain = 7,
    .bucket = libdl_buckets,
    .chain = libdl_chains,
};
    
