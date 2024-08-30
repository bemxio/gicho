#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void* memset(void* s, int c, size_t n);
void* memcpy(void* restrict s1, const void* restrict s2, size_t n);

size_t strlen(const char* s);
char* strcpy(char* restrict s1, const char* restrict s2);
int strcmp(const char* s1, const char* s2);

#endif