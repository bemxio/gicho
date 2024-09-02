#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>

char getchar();
char _putchar(char c, char attr);

#define putchar(c, ...) _putchar(c, ##__VA_ARGS__, 0x07)

char* gets(char* s);
int puts(const char *s);

#endif