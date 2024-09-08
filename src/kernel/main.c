#include "commands.h"

#include "lib/stdio.h"
#include "lib/string.h"

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
        } else if (strcmp(token, "peek") == 0) {
            peek(token);
        } else if (strcmp(token, "poke") == 0) {
            poke(token);
        } else if (strcmp(token, "int") == 0) {
            _int(token);
        } else if (strcmp(token, "read") == 0) {
            read(token);
        } else if (strcmp(token, "write") == 0) {
            write(token);
        } else if (input[0] == '\0') {
            continue;
        } else {
            puts("Command not found.\r\n");
        }
    }
}