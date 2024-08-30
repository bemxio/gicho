#include "stdio.h"

char getchar() {
    char c;

    __asm__ (
        "mov $0x00, %%ah\n"
        "int $0x16\n"
        "mov %%al, %0\n"
        : "=r" (c)
    );

    return c;
}

char putchar(char c) {
    __asm__ (
        "mov $0x0e, %%ah\n"
        "mov %0, %%al\n"
        "int $0x10\n"
        :: "r" (c)
    );

    return c;
}

char* gets(char* s) {
    char c;
    size_t i = 0;

    while ((c = getchar()) != '\r') {
        if (c == '\b') {
            if (i == 0) {
                continue;
            }

            s[--i] = '\0';

            putchar('\b');
            putchar(' ');
            putchar('\b');

            continue;
        }

        s[i] = c;
        putchar(c);

        i++;
    }

    s[i] = '\0';

    putchar('\r');
    putchar('\n');

    return s;
}

int puts(const char* s) {
    size_t i = 0;

    while (s[i] != '\0') {
        putchar(s[i++]);
    }

    //putchar('\r');
    //putchar('\n');

    return i;
}