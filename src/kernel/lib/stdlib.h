#ifndef STDLIB_H
#define STDLIB_H

#include "string.h"

int atoi(const char* nptr);
long atol(const char* nptr);
unsigned long atoul(const char* nptr);

char* itoa(int n, char* buf);
//char* ltoa(long n, char* buf);
//char* ultoa(unsigned long n, char* buf);

#endif