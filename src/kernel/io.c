#include "io.h"

void putchar(char c) {
    __asm__ (
        "movb $0x0e, %%ah\n"
        "movb %0, %%al\n"
        "int $0x10\n"
        :: "r" (c)
        : "ah", "al"
    );
}

void puts(const char* s) {
    while (*s) {
        putchar(*s++);
    }
}