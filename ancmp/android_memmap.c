#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include "ancmp_stdint.h"
#include <stdio.h>
#include "android_memmap.h"
#include <string.h>
#include "android_pthread_mutex.h"

static unsigned char is_initialized = 0;
static size_t page_size = 4096;
size_t page_count = 0;
static size_t alloc_size = 0;
static void *memory_map = NULL;
static unsigned char *free_table = NULL;
static android_pthread_mutex_t mapper_mtx = ANDROID_PTHREAD_MUTEX_INITIALIZER;

int memmap_init(size_t map_len, size_t pg_len) {
    if (!is_initialized) {
        page_size = pg_len;
        page_count = (map_len / pg_len) + ((map_len % pg_len) ? 1 : 0);
        alloc_size = page_count * pg_len;
#ifdef _WIN32
        memory_map = VirtualAlloc(NULL, alloc_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (memory_map == NULL) {
#else
        memory_map = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (memory_map == MAP_FAILED) {
#endif
            puts("failed to allocate");
            return 0;
        }
        free_table = malloc(page_count);
        if (!free_table) {
#ifdef _WIN32
            VirtualFree(memory_map, 0, MEM_RELEASE);
#else
            munmap(memory_map, alloc_size);
#endif
            return 0;
        }
        memset(free_table, 0, page_count);
        is_initialized = 1;
        return 1;
    }
    return 0;
}

static long find_free_off(size_t pg_n) {
    long ret = 0;
    size_t cnt = 0;
    size_t i;
    for (i = 0; i < page_count; ++i) {
        if (cnt == pg_n) {
            return ret;
        }
        if (!free_table[i]) {
            ++cnt;
        } else {
            ret = i + 1;
            cnt = 0;
        }
    }
    return -1;
}

static int set_alloced(long pg_off, size_t pg_n, unsigned char b) {
    size_t i;
    if (pg_off >= page_count || pg_off + pg_n > page_count) {
        return 0;
    }
    for (i = pg_off; i < pg_off + pg_n; ++i) {
        free_table[i] = b;
    }
    return 1;
}

void *memmap_alloc(void *addr, size_t len, unsigned char overwrite) {
    size_t pg_n = (len / page_size) + ((len % page_size) ? 1 : 0);
    long off = -1;
    void *ret = NULL;
    if (!overwrite) {
        android_pthread_mutex_lock(&mapper_mtx);
        off = find_free_off(pg_n);
        if (off == -1) {
            android_pthread_mutex_unlock(&mapper_mtx);
            return NULL;
        }
        ret = (void *)((char *)memory_map + (off * page_size));
    } else {
        if ((uintptr_t)addr < (uintptr_t)memory_map || (uintptr_t)addr+len > (uintptr_t)memory_map+alloc_size) {
            return NULL;
        }
        off = ((uintptr_t)addr - (uintptr_t)memory_map) / page_size;
        ret = addr;
        android_pthread_mutex_lock(&mapper_mtx);
    }
    set_alloced(off, pg_n, 1);
    android_pthread_mutex_unlock(&mapper_mtx);
    return ret;
}

int memmap_dealloc(void *addr, size_t len) {
    size_t pg_n;

    if ((uintptr_t)addr < (uintptr_t)memory_map || (uintptr_t)addr+len > (uintptr_t)memory_map+alloc_size) {
        return 0;
    }
    pg_n = (len / page_size) + ((len % page_size) ? 1 : 0);
    android_pthread_mutex_lock(&mapper_mtx);
    set_alloced(((uintptr_t)addr - (uintptr_t)memory_map) / page_size, pg_n, 0);
    android_pthread_mutex_unlock(&mapper_mtx);
    return 1;
}
