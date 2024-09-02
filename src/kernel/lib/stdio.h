#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>

char getchar();
char _putchar(char c, char attr, ...);

char* gets(char* s);
int _puts(const char *s, char attr, ...);

#define putchar(c, ...) _putchar(c, ##__VA_ARGS__, 0x07)
#define puts(s, ...) _puts(s, ##__VA_ARGS__, 0x07)

#endif