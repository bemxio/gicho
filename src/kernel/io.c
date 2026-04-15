#include "io.h"

char getchar() {
    char c;

    __asm__ (
        "movb $0x00, %%ah\n"
        "int $0x16\n"
        "movb %%al, %0\n"
        : "=r" (c)
        :: "ah", "al"
    );

    return c;
}

char* gets(char* buf) {
    char c;
    size_t i = 0;

    while ((c = getchar()) != '\r') {
        if (c == '\b') {
            if (i == 0)
                continue;

            buf[--i] = '\0';

            putchar('\b');
            putchar(' ');
            putchar('\b');

            continue;
        }

        buf[i] = c;
        putchar(c);

        i++;
    }

    buf[i] = '\0';

    putchar('\r');
    putchar('\n');

    return buf;
}

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