#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define KERNEL_SIZE *((unsigned char*)0x7e01) // kernel size in sectors

void* memset(void* s, int c, size_t n);
void* memcpy(void* restrict s1, const void* restrict s2, size_t n);

void* malloc(size_t size);

#endif