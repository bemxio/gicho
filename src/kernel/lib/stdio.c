#include "stdio.h"

char getchar() {
    char c;

    __asm__ (
        "mov $0x00, %%ah\n"
        "int $0x16\n"
        "mov %%al, %0\n"
        : "=r" (c)
        :: "ah", "al"
    );

    return c;
}

char _putchar(char c, char attr) {
    char x;
    char y;

    __asm__ (
        "mov $0x03, %%ah\n"
        "xor %%bh, %%bh\n"
        "int $0x10\n"
        "mov %%dl, %0\n"
        "mov %%dh, %1\n"
        : "=r" (x), "=r" (y)
        :: "ah", "bh", "dl", "dh"
    );

    if (c >= 32 && c <= 126) {
        __asm__ (
            "mov $0x09, %%ah\n"
            "xor %%bh, %%bh\n"
            "mov $0x01, %%cx\n"
            "mov %0, %%al\n"
            "mov %1, %%bl\n"
            "int $0x10\n"
            :: "r" (c), "r" (attr)
            : "ah", "bh", "cx", "al", "bl"
        );

        x++;
    } else if (c == '\r') {
        x = 0;
    } else if (c == '\n') {
        y++;
    } else if (c == '\b') {
        x--;
    }

    __asm__ (
        "mov $0x02, %%ah\n"
        "xor %%bh, %%bh\n"
        "mov %0, %%dl\n"
        "mov %1, %%dh\n"
        "int $0x10\n"
        :: "r" (x), "r" (y)
        : "ah", "bh", "dl", "dh"
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