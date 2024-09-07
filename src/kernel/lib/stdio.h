#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>

char getchar();
char __putchar(char c, char attr, ...);

char* gets(char* s);
int __puts(const char *s, char attr, ...);

#define DEFAULT_ATTRIBUTE 0x07

#define putchar(c, ...) __putchar(c, ##__VA_ARGS__, DEFAULT_ATTRIBUTE)
#define puts(s, ...) __puts(s, ##__VA_ARGS__, DEFAULT_ATTRIBUTE)

#endif