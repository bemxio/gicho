#include "commands.h"

void cmd_print(char* token) {
    while ((token = strtok(NULL, " ")) != NULL) {
        puts(token);
        putchar(' ');
    }

    putchar('\r');
    putchar('\n');
}

void cmd_peek(char* token) {
    uint32_t address;

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("peek: Address not specified.\r\n"); return;
    }

    address = atoi(token);

    uint16_t segment = address >> 16;
    uint16_t offset = address & 0xffff;

    uint8_t value;
    char buffer[4];

    __asm__ (
        "movw %1, %%ax\n"
        "movw %2, %%si\n"
        "movw %%ax, %%es\n"
        "movb %%es:(%%si), %0\n"
        : "=g" (value)
        : "g" (segment), "g" (offset)
        : "ax", "si", "es"
    );

    itoa(value, buffer, 10);
    puts(buffer);

    putchar('\r');
    putchar('\n');
}

void cmd_poke(char* token) {
    uint32_t address;
    uint8_t value;

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("poke: Address not specified.\r\n"); return;
    }

    address = atoi(token);

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("poke: Value not specified.\r\n"); return;
    }

    value = atoi(token);

    uint16_t segment = address >> 16;
    uint16_t offset = address & 0xffff;

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

void cmd_int(char* token) {
    uint8_t interrupt;

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("int: Interrupt not specified.\r\n"); return;
    }

    char* names[6] = {"ax", "bx", "cx", "dx", "si", "di"};
    uint16_t registers[6] = {0};

    interrupt = atoi(token);

    while ((token = strtok(NULL, " ")) != NULL) {
        uint16_t value = atoi(token + 3);

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

    for (uint8_t i = 0; i < 6; i++) {
        puts(names[i]);
        putchar('=');

        itoa(registers[i], buffer, 16);
        puts(buffer);

        putchar(' ');
    }

    putchar('\r');
    putchar('\n');
}

void cmd_read(char* token) {
    uint8_t drive;
    uint8_t amount;
    uint32_t position;
    uint32_t address;

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("read: Drive number not specified.\r\n"); return;
    }

    drive = atoi(token);

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("read: Sector amount not specified.\r\n"); return;
    }

    amount = atoi(token);

    if ((token = strtok(NULL, " ")) != NULL) {
        position = atoi(token);
    } else {
        position = 0;
    }

    if ((token = strtok(NULL, " ")) != NULL) {
        address = atoi(token);
    } else {
        address = 0x7e00;
    }

    if (position > 1033199) {
        puts("read: Offset out of range.\r\n"); return;
    }

    uint16_t segment = address >> 16;
    uint16_t offset = address & 0xffff;

    uint16_t cylinders = position / (16 * 63);
    uint8_t heads = (position / 63) % 16;
    uint8_t sectors = (position % 63) + 1;

    uint8_t ch = cylinders & 0xff;
    uint8_t cl = ((cylinders >> 2) & 0xc0) | sectors;

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

    uint8_t status;

    __asm__ (
        "movb %%ah, %0\n"
        : "=g" (status)
        :: "al"
    );

    if (status != 0) {
        puts("read: Failed with error code ");

        itoa(status, token, 16);
        puts(token);

        puts(".\r\n");
    }
}

void cmd_write(char* token) {
    uint8_t drive;
    uint8_t amount;
    uint32_t address;
    uint32_t position;

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("write: Drive number not specified.\r\n"); return;
    }

    drive = atoi(token);

    if ((token = strtok(NULL, " ")) == NULL) {
        puts("write: Sector amount not specified.\r\n"); return;
    }

    amount = atoi(token);

    if ((token = strtok(NULL, " ")) != NULL) {
        address = atoi(token);
    } else {
        address = 0x7e00;
    }

    if ((token = strtok(NULL, " ")) != NULL) {
        position = atoi(token);
    } else {
        position = 0;
    }

    if (position > 1033199) {
        puts("write: Offset out of range.\r\n"); return;
    }

    uint16_t segment = address >> 16;
    uint16_t offset = address & 0xffff;

    uint16_t cylinders = position / (16 * 63);
    uint8_t heads = (position / 63) % 16;
    uint8_t sectors = (position % 63) + 1;

    uint8_t ch = cylinders & 0xff;
    uint8_t cl = ((cylinders >> 2) & 0xc0) | sectors;

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

    uint8_t status;

    __asm__ (
        "movb %%ah, %0\n"
        : "=g" (status)
        :: "al"
    );

    if (status != 0) {
        puts("write: Failed with error code ");

        itoa(status, token, 16);
        puts(token);

        puts(".\r\n");
    }
}

command_t commands[] = {
    {"print", cmd_print},
    {"clear", clear},
    {"peek", cmd_peek},
    {"poke", cmd_poke},
    {"int", cmd_int},
    {"read", cmd_read},
    {"write", cmd_write},
    {NULL, NULL}
};