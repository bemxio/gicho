#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char* s);
int strcmp(const char* s1, const char* s2);
char* strrev(char* s);
char* strtok(char* s, const char* delim);

int atoi(const char* str);
char* itoa(int n, char* buf, char base);

#endif