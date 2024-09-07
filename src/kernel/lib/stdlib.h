#ifndef STDLIB_H
#define STDLIB_H

#include "string.h"

unsigned long atoul(const char* nptr);
char* ultoa(unsigned long n, char* buf);

#endif