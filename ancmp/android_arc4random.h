#ifndef ANCMP_ANDROID_ARC4RANDOM_H
#define ANCMP_ANDROID_ARC4RANDOM_H

#include "ancmp_stdint.h"
#include <stdlib.h>

uint8_t android_arc4_getbyte(void);

void android_arc4random_stir(void);

void android_arc4random_addrandom(unsigned char *dat, int datlen);

uint32_t android_arc4random(void);

void android_arc4random_buf(void *_buf, size_t n);

uint32_t android_arc4random_uniform(uint32_t upper_bound);

#endif