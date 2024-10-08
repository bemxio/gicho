#include "commands.h"

#include "lib/stdio.h"
#include "lib/stdlib.h"
#include "lib/string.h"

#include <stdbool.h>

void help() {
    puts("Available commands:\r\n");
    puts("  help - Display this message.\r\n");
    puts("  echo - Echo the input.\r\n");
    puts("  clear - Clear the screen.\r\n");
    puts("  peek - Read a byte at a specified address in memory.\r\n");
    puts("  poke - Write a byte at a specified address in memory.\r\n");
    puts("  int - Call a BIOS interrupt with optional register values.\r\n");
    puts("  read - Read sectors from a specified disk.\r\n");
    puts("  write - Write sectors to a specified disk.\r\n");
    puts("  run - Run a program at a specified address in memory.\r\n");
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
        "int $0x10\n"
        :: "g" (mode)
        : "ah", "al"
    );
}

void peek(char* token) {
    unsigned long address;

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: peek <address>\r\n"); return;
    }

    address = atoul(token);

    unsigned int segment = address >> 16;
    unsigned int offset = address & 0xffff;

    unsigned char value;
    char buffer[4];

    __asm__ (
        "movw %1, %%ax\n"
        "movw %2, %%si\n"
        "movw %%ax, %%es\n"
        "movb %%es:(%%si), %0\n"
        : "=r" (value) 
        : "g" (segment), "g" (offset)
        : "ax", "si", "es"
    );

    ultoa(value, buffer, 10);
    puts(buffer);

    putchar('\r');
    putchar('\n');
}

void poke(char* token) {
    unsigned long address;
    unsigned char value;

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: poke <address> <value>\r\n"); return;
    }

    address = atoul(token);

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: poke <address> <value>\r\n"); return;
    }

    value = atoul(token);

    unsigned int segment = address >> 16;
    unsigned int offset = address & 0xffff;

    __asm__ (
        "movw %0, %%ax\n"
        "movw %1, %%si\n"
        "movb %2, %%bl\n"
        "movw %%ax, %%es\n"
        "movb %%bl, %%es:(%%si)\n"
        :: "g" (segment), "g" (offset), "g" (value)
        : "ax", "si", "bl", "es"
    );
}

void _int(char* token) {
    unsigned char interrupt;

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: int <interrupt> [registers]\r\n"); return;
    }

    char* names[6] = {"ax", "bx", "cx", "dx", "si", "di"};
    unsigned int registers[6] = {0};

    interrupt = atoul(token);

    while ((token = strtok(NULL, " ")) != NULL) {
        unsigned int value = atoul(token + 3);

        switch (token[0]) {
            case 'a':
                switch (token[1]) {
                    case 'x':
                        registers[0] = value; break;
                    case 'h':
                        registers[0] += value << 8; break;
                    case 'l':
                        registers[0] += value & 0xff; break;
                }

                break;
            case 'b':
                switch (token[1]) {
                    case 'x':
                        registers[1] = value; break;
                    case 'h':
                        registers[1] += value << 8; break;
                    case 'l':
                        registers[1] += value & 0xff; break;
                    /*
                    case 'p':
                        bp = value; break;
                    */
                }

                break;
            case 'c':
                switch (token[1]) {
                    case 'x':
                        registers[2] = value; break;
                    case 'h':
                        registers[2] += value << 8; break;
                    case 'l':
                        registers[2] += value & 0xff; break;
                }

                break;
            case 'd':
                switch (token[1]) {
                    case 'x':
                        registers[3] = value; break;
                    case 'h':
                        registers[3] += value << 8; break;
                    case 'l':
                        registers[3] += value & 0xff; break;
                    case 'i':
                        registers[5] = value; break;
                }

                break;
            case 's':
                switch (token[1]) {
                    /*
                    case 'p':
                        sp = value; break;
                    */
                    case 'i':
                        registers[4] = value; break;
                }

                break;
        }
    }

    __asm__ (
        "movb %0, %%al\n"
        "movb %%al, %%cs:interrupt+1\n"
        "movw %1, %%ax\n"
        "movw %2, %%bx\n"
        "movw %3, %%cx\n"
        "movw %4, %%dx\n"
        "movw %5, %%si\n"
        "movw %6, %%di\n"
        "interrupt:\n\t"
        "int $0x00\n"
        :: "g" (interrupt), \
           "g" (registers[0]), "g" (registers[1]), "g" (registers[2]), \
           "g" (registers[3]), "g" (registers[4]), "g" (registers[5])
        : "cs", "ax", "bx", "cx", "dx", "si", "di"
    );

    __asm__ (
        "movw %%ax, %0\n"
        "movw %%bx, %1\n"
        "movw %%cx, %2\n"
        "movw %%dx, %3\n"
        "movw %%si, %4\n"
        "movw %%di, %5\n"
        : "=g" (registers[0]), "=g" (registers[1]), "=g" (registers[2]), \
          "=g" (registers[3]), "=g" (registers[4]), "=g" (registers[5])
        :: "ax", "bx", "cx", "dx", "si", "di"
    );

    char buffer[7];

    for (unsigned char i = 0; i < 6; i++) {
        puts(names[i]);
        putchar('=');

        ultoa(registers[i], buffer, 16);
        puts(buffer);

        putchar(' ');
    }

    putchar('\r');
    putchar('\n');
}

void read(char* token) {
    unsigned char drive;
    unsigned char amount;
    unsigned long position;
    unsigned long address;

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: read <drive number> <sector amount> [offset] [memory address]\r\n"); return;
    }

    drive = atoul(token);

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: read <drive number> <sector amount> [offset] [memory address]\r\n"); return;
    }

    amount = atoul(token);

    if ((token = strtok(NULL, " ")) != NULL) {
        position = atoul(token);
    } else {
        position = 0;
    }

    if ((token = strtok(NULL, " ")) != NULL) {
        address = atoul(token);
    } else {
        address = 0x7e00;
    }

    if (position > 1033199) {
        puts("Offset out of range.\r\n"); return;
    }

    unsigned int segment = address >> 16;
    unsigned int offset = address & 0xffff;

    unsigned int cylinders = position / (16 * 63);
    unsigned char heads = (position / 63) % 16;
    unsigned char sectors = (position % 63) + 1;

    unsigned char ch = cylinders & 0xff;
    unsigned char cl = ((cylinders >> 2) & 0xc0) | sectors;

    __asm__ (
        "movb $0x02, %%ah\n"
        "movb %0, %%al\n"
        "movb %1, %%ch\n"
        "movb %2, %%cl\n"
        "movb %3, %%dh\n"
        "movb %4, %%dl\n"
        "movw %5, %%bx\n"
        "movw %%bx, %%es\n"
        "movw %6, %%bx\n"
        "int $0x13\n"
        :: "g" (amount), "g" (ch), "g" (cl), "g" (heads), "g" (drive), "g" (segment), "g" (offset)
        : "ah", "al", "ch", "cl", "dh", "dl", "bx", "es"
    );

    unsigned char status;

    __asm__ (
        "movb %%ah, %0\n"
        : "=r" (status)
        :: "al"
    );

    if (status != 0) {
        puts("Disk read failed with error code ");

        ultoa(status, token, 16);
        puts(token);
        
        putchar('.');
        putchar('\r');
        putchar('\n');
    }
}

void write(char* token) {
    unsigned char drive;
    unsigned char amount;
    unsigned long address;
    unsigned long position;

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: write <drive number> <sector amount> [memory address] [offset]\r\n"); return;
    }

    drive = atoul(token);

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("Usage: write <drive number> <sector amount> [memory address] [offset]\r\n"); return;
    }

    amount = atoul(token);

    if ((token = strtok(NULL, " ")) != NULL) {
        address = atoul(token);
    } else {
        address = 0x7e00;
    }

    if ((token = strtok(NULL, " ")) != NULL) {
        position = atoul(token);
    } else {
        position = 0;
    }

    if (position > 1033199) {
        puts("Offset out of range.\r\n"); return;
    }

    unsigned int segment = address >> 16;
    unsigned int offset = address & 0xffff;

    unsigned int cylinders = position / (16 * 63);
    unsigned char heads = (position / 63) % 16;
    unsigned char sectors = (position % 63) + 1;

    unsigned char ch = cylinders & 0xff;
    unsigned char cl = ((cylinders >> 2) & 0xc0) | sectors;

    __asm__ (
        "movb $0x03, %%ah\n"
        "movb %0, %%al\n"
        "movb %1, %%ch\n"
        "movb %2, %%cl\n"
        "movb %3, %%dh\n"
        "movb %4, %%dl\n"
        "movw %5, %%bx\n"
        "movw %%bx, %%es\n"
        "movw %6, %%bx\n"
        "int $0x13\n"
        :: "g" (amount), "g" (ch), "g" (cl), "g" (heads), "g" (drive), "g" (segment), "g" (offset)
        : "ah", "al", "ch", "cl", "dh", "dl", "bx", "es"
    );

    unsigned char status;

    __asm__ (
        "movb %%ah, %0\n"
        : "=r" (status)
        :: "al"
    );

    if (status != 0) {
        puts("Disk write failed with error code ");

        ultoa(status, token, 16);
        puts(token);

        putchar('.');
        putchar('\r');
        putchar('\n');
    }
}

void run(char* token) {
    unsigned long address;

    if ((token = strtok(NULL, " ")) == NULL) {
        address = 0x7e00;
    } else {
        address = atoul(token);
    }

    unsigned int segment = address >> 16;
    unsigned int offset = address & 0xffff;

    __asm__ (
        "mov $0x0000, %%ax\n"
        "mov $return_from_call, %%bx\n"
        "push %%ax\n"
        "push %%bx\n"
        "push %0\n"
        "push %1\n"
        "retf\n"
        "return_from_call:\n"
        :: "g" (segment), "g" (offset)
        : "ax", "bx"
    );
}