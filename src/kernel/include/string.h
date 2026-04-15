#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>

size_t strlen(const char* s);
int strcmp(const char* s1, const char* s2);
char* strrev(char* s);
char* strtok(char* s, const char* delim);

unsigned long atoul(const char* str);
char* ultoa(unsigned long n, char* buf, char base);

#endif