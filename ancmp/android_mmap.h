#ifndef ANCMP_ANDROID_MMAP_H
#define ANCMP_ANDROID_MMAP_H

#include <stdlib.h>
#include <sys/types.h>

#define ANDROID_PROT_READ 0x1  
#define ANDROID_PROT_WRITE 0x2  
#define ANDROID_PROT_EXEC 0x4  
#define ANDROID_PROT_SEM 0x8  
#define ANDROID_PROT_NONE 0x0  
#define ANDROID_PROT_GROWSDOWN 0x01000000  
#define ANDROID_PROT_GROWSUP 0x02000000  

#define ANDROID_MAP_SHARED 0x01  
#define ANDROID_MAP_PRIVATE 0x02  
#define ANDROID_MAP_TYPE 0x0f  
#define ANDROID_MAP_FIXED 0x10  
#define ANDROID_MAP_ANONYMOUS 0x20  

#define ANDROID_MS_ASYNC 1  
#define ANDROID_MS_INVALIDATE 2  
#define ANDROID_MS_SYNC 4  

#define ANDROID_MADV_NORMAL 0  
#define ANDROID_MADV_RANDOM 1  
#define ANDROID_MADV_SEQUENTIAL 2  
#define ANDROID_MADV_WILLNEED 3  
#define ANDROID_MADV_DONTNEED 4  

#define ANDROID_MADV_REMOVE 9  
#define ANDROID_MADV_DONTFORK 10  
#define ANDROID_MADV_DOFORK 11  

#define ANDROID_MAP_ANON MAP_ANONYMOUS
#define ANDROID_MAP_FILE 0

#define ANDROID_MAP_FAILED ((void *)-1)

#ifdef _WIN32
void *android_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

int android_munmap(void *addr, size_t length);
#else
#include <sys/mman.h>
#define android_mmap mmap
#define android_munmap munmap
#endif

#endif