#include "lib/stdio.h"
#include "lib/stdlib.h"
#include "lib/string.h"

#include <stdbool.h>

void help() {
    puts("Available commands:\r\n");
    puts("  help - Display this message.\r\n");
    puts("  echo - Echo the input.\r\n");
    puts("  clear - Clear the screen.\r\n");
    //puts("  peek - Read a byte at a specified address.\r\n");
    //puts("  poke - Write a byte in a specified address.\r\n");
}

void echo(char* token) {
    while ((token = strtok(NULL, " ")) != NULL) {
        puts(token);
        putchar(' ');
    }

    putchar('\r');
    putchar('\n');
}

void clear() {
    char mode = *(char*)0x0449;

    __asm__ (
        "mov $0x00, %%ah\n"
        "mov %0, %%al\n"
        "int $0x10"
        :: "r" (mode)
        : "ah", "al"
    );
}

void main() {
    char input[127];
    char* token;

    clear();

    puts("Welcome to Gicho!\r\n");
    puts("Type `help` for a list of commands.\r\n");

    while (true) {
        puts("> ");
        gets(input);

        token = strtok(input, " ");

        if (strcmp(token, "help") == 0) {
            help();
        } else if (strcmp(token, "echo") == 0) {
            echo(token);
        } else if (strcmp(token, "clear") == 0) {
            clear();
        } else if (input[0] == '\0') {
            continue;
        } else {
            puts("Command not found.\r\n");
        }
    }
}