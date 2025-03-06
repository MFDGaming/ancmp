#pragma once

#include <stdlib.h>

int memmap_init(size_t map_len, size_t pg_len);

void *memmap_alloc(void *addr, size_t len, unsigned char overwrite);

int memmap_dealloc(void *addr, size_t len);