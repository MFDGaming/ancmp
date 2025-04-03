/*
 * Copyright (C) 2008-2010 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef ANCMP_LINKER_DEBUG_H
#define ANCMP_LINKER_DEBUG_H

#include <stdio.h>

#ifndef LINKER_DEBUG
#error LINKER_DEBUG should be defined to either 1 or 0 in Android.mk
#endif

/* set LINKER_DEBUG_TO_LOG to 1 to send the logs to logcat,
 * or 0 to use stdout instead.
 */
#define LINKER_DEBUG_TO_LOG  1
#define TRACE_DEBUG          1
#define DO_TRACE_LOOKUP      0
#define DO_TRACE_RELO        0
#define TIMING               0
#define STATS                0
#define COUNT_PAGES          0

enum {
    LOOKUP,
    RELO
};

/*********************************************************************
 * You shouldn't need to modify anything below unless you are adding
 * more debugging information.
 *
 * To enable/disable specific debug options, change the defines above
 *********************************************************************/


/*********************************************************************/
#undef TRUE
#undef FALSE
#define TRUE                 1
#define FALSE                0

void PRINT(const char *fmt, ...);

void INFO(const char *fmt, ...);

void TRACE(const char *fmt, ...);

void WARN(const char *fmt, ...);

void ERROR_O(const char *fmt, ...);

void DEBUG(const char *fmt, ...);

void TRACE_TYPE(int type, const char *fmt, ...);

#if STATS
#define RELOC_ABSOLUTE        0
#define RELOC_RELATIVE        1
#define RELOC_COPY            2
#define RELOC_SYMBOL          3
#define NUM_RELOC_STATS       4

struct _link_stats {
    int reloc[NUM_RELOC_STATS];
};
extern struct _link_stats linker_stats;

#define COUNT_RELOC(type)                                 \
        do { if (type >= 0 && type < NUM_RELOC_STATS) {   \
                linker_stats.reloc[type] += 1;            \
             } else  {                                    \
                PRINT("Unknown reloc stat requested\n");  \
             }                                            \
           } while(0)
#else /* !STATS */
#define COUNT_RELOC(type)     do {} while(0)
#endif /* STATS */

#if COUNT_PAGES
extern unsigned bitmask[];
#define MARK(offset)         do {                                        \
        bitmask[((offset) >> 12) >> 3] |= (1 << (((offset) >> 12) & 7)); \
    } while(0)
#else
#define MARK(x)              do {} while (0)
#endif

#define DEBUG_DUMP_PHDR(phdr, name, pid) do { \
        DEBUG("%5d %s (phdr = 0x%08x)\n", (pid), (name), (unsigned)(phdr));   \
        DEBUG("\t\tphdr->offset   = 0x%08x\n", (unsigned)((phdr)->p_offset)); \
        DEBUG("\t\tphdr->p_vaddr  = 0x%08x\n", (unsigned)((phdr)->p_vaddr));  \
        DEBUG("\t\tphdr->p_paddr  = 0x%08x\n", (unsigned)((phdr)->p_paddr));  \
        DEBUG("\t\tphdr->p_filesz = 0x%08x\n", (unsigned)((phdr)->p_filesz)); \
        DEBUG("\t\tphdr->p_memsz  = 0x%08x\n", (unsigned)((phdr)->p_memsz));  \
        DEBUG("\t\tphdr->p_flags  = 0x%08x\n", (unsigned)((phdr)->p_flags));  \
        DEBUG("\t\tphdr->p_align  = 0x%08x\n", (unsigned)((phdr)->p_align));  \
    } while (0)

#endif
