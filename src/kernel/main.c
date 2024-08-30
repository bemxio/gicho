#include "lib/stdio.h"
#include "lib/string.h"

#include <stdbool.h>

void vidmodset(char mode) {
    __asm__ (
        "mov $0x00, %%ah\n"
        "mov %0, %%al\n"
        "int $0x10"
        :: "r" (mode)
    );
}

int main() {
    char input[127];
    char* token;

    vidmodset(0x03);

    puts("Gicho v1.0.0\r\n");
    puts("Type `help` for a list of commands.\r\n");

    while (true) {
        puts("> ");
        gets(input);

        token = strtok(input, " ");

        if (strcmp(token, "help") == 0) {
            puts("Available commands:\r\n");
            puts("  help - Display this message.\r\n");
            //puts("  echo - Echo the input.\r\n");
        } else if (input[0] == '\0') {
            continue;
        } else {
            puts("Command not found.\r\n");
        }
    }

    return 0;
}