#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void* memset(void* s, int c, size_t n);
void* memcpy(void* restrict s1, const void* restrict s2, size_t n);

void* malloc(size_t size);

#endif