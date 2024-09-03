#include "lib/stdio.h"
#include "lib/stdlib.h"
#include "lib/string.h"

#include <stdbool.h>

void help() {
    puts("Available commands:\r\n");
    puts("  help - Display this message.\r\n");
    puts("  echo - Echo the input.\r\n");
    puts("  clear - Clear the screen.\r\n");
    puts("  peek - Read a byte at a specified address.\r\n");
    puts("  poke - Write a byte at a specified address.\r\n");
    puts("  int - Call a BIOS interrupt with optional register values.\r\n");
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

void peek(char* token) {
    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: peek <address>\r\n"); return;
    }

    unsigned long address = atoul(token);

    unsigned int segment = address >> 16;
    unsigned int offset = address & 0xffff;

    unsigned char value;
    char* buffer;

    __asm__ (
        "movw %1, %%ax\n"
        "movw %2, %%si\n"
        "movw %%ax, %%es\n"
        "movb %%es:(%%si), %0\n"
        : "=r" (value) 
        : "r" (segment), "r" (offset)
        : "ax", "si", "es"
    );

    itoa(value, buffer);
    puts(buffer);

    putchar('\r');
    putchar('\n');
}

void poke(char* token) {
    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: poke <address> <value>\r\n"); return;
    }

    unsigned long address = atoul(token);

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: poke <address> <value>\r\n"); return;
    }

    unsigned char value = atoul(token);

    unsigned int segment = address >> 16;
    unsigned int offset = address & 0xffff;

    __asm__ (
        "movw %0, %%ax\n"
        "movw %1, %%si\n"
        "movb %2, %%bl\n"
        "movw %%ax, %%es\n"
        "movb %%bl, %%es:(%%si)\n"
        :: "r" (segment), "r" (offset), "r" (value)
        : "ax", "si", "bl", "es"
    );
}

void _int(char* token) {
    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: int <interrupt> [registers]\r\n"); return;
    }

    unsigned char interrupt = atoul(token);
    unsigned int value;

    unsigned int ax = 0;
    unsigned int bx = 0;
    unsigned int cx = 0;
    unsigned int dx = 0;

    while ((token = strtok(NULL, " ")) != NULL) {
        value = atoul(token + 3);

        switch (token[0]) {
            case 'a':
                switch (token[1]) {
                    case 'x':
                        ax = value; break;
                    case 'h':
                        ax += value << 8; break;
                    case 'l':
                        ax += value & 0xff; break;
                }

                break;
            case 'b':
                switch (token[1]) {
                    case 'x':
                        bx = value; break;
                    case 'h':
                        bx += value << 8; break;
                    case 'l':
                        bx += value & 0xff; break;
                    /*
                    case 'p':
                        bp = value; break;
                    */
                }

                break;
            case 'c':
                switch (token[1]) {
                    case 'x':
                        cx = value; break;
                    case 'h':
                        cx += value << 8; break;
                    case 'l':
                        cx += value & 0xff; break;
                }

                break;
            case 'd':
                switch (token[1]) {
                    case 'x':
                        dx = value; break;
                    case 'h':
                        dx += value << 8; break;
                    case 'l':
                        dx += value & 0xff; break;
                    /*
                    case 'i':
                        di = value; break;
                    */
                }

                break;
            /*
            case 's':
                switch (token[1]) {
                    case 'p':
                        sp = value; break;
                    case 'i':
                        si = value; break;
                }

                break;
            */
        }
    }

    __asm__ (
        "movb %0, %%al\n"
        "movb %%al, %%cs:interrupt+1\n"
        "movw %1, %%ax\n"
        "movw %2, %%bx\n"
        "movw %3, %%cx\n"
        "movw %4, %%dx\n"
        "interrupt:\n\t"
        "int $0x00\n"
        :: "g" (interrupt), "g" (ax), "g" (bx), "g" (cx), "g" (dx)
        : "cs", "ax", "bx", "cx", "dx"
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
        } else if (strcmp(token, "peek") == 0) {
            peek(token);
        } else if (strcmp(token, "poke") == 0) {
            poke(token);
        } else if (strcmp(token, "int") == 0) {
            _int(token);
        } else if (input[0] == '\0') {
            continue;
        } else {
            puts("Command not found.\r\n");
        }
    }
}