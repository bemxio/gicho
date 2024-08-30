#include "stdio.h"

char getchar() {
    char character;

    __asm__ (
        "mov $0x00, %%ah\n"
        "int $0x16\n"
        "mov %%al, %0\n"
        : "=r" (character)
    );

    return character;
}

char putchar(char character) {
    __asm__ (
        "mov $0x0e, %%ah\n"
        "mov %0, %%al\n"
        "int $0x10\n"
        :: "r" (character)
    );

    return character;
}

char* gets(char* string) {
    char character;
    int index = 0;

    while ((character = getchar()) != '\r') {
        if (character == '\b') {
            if (index == 0) {
                continue;
            }

            string[--index] = '\0';

            putchar('\b');
            putchar(' ');
            putchar('\b');

            continue;
        }

        string[index] = character;
        putchar(character);

        index++;
    }

    string[index] = '\0';

    putchar('\r');
    putchar('\n');

    return string;
}

int puts(const char* string) {
    int index = 0;

    while (string[index] != '\0') {
        putchar(string[index++]);
    }

    putchar('\r');
    putchar('\n');

    return index;
}