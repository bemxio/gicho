#ifndef INT_H
#define INT_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

uint32_t atoi(const char* str);
char* itoa(uint32_t n, char* buf, char base);

//bool isdigit(char c);
bool isnumeric(const char* s);

//void sort(uint16_t* arr, size_t n);

#endif