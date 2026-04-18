#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stddef.h>

int strcmp(const char* s1, const char* s2);
void* strcpy(char* restrict s1, const char* restrict s2);
size_t strlen(const char* s);
char* strrev(char* s);
char* strtok(char* s, const char* delim);

int atoi(const char* str);
char* itoa(int n, char* buf, char base);

//bool isdigit(char c);
bool isnumeric(const char* s);

#endif