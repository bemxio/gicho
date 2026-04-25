#include "commands.h"

#include "../lib/int.h"
#include "../lib/io.h"
#include "../lib/string.h"

#include <stdint.h>

// utility functions
shell_cmd_t* shell_cmd_find(char* name) {
    tolower(name); // TODO: duplicate to avoid modifying the original string

    for (shell_cmd_t* cmd = shell_cmds; cmd->name != NULL; cmd++) {
        if (strcmp(name, cmd->name) == 0)
            return cmd;
    }

    return NULL;
}

// commands
void shell_cmd_print(shell_t* shell) {
    while ((shell->token = strtok(NULL, " ")) != NULL) {
        if (shell->token[0] == '$') {
            if (shell->token[1] == '$') {
                puts(shell->token + 1);
            } else {
                shell_var_t* variable = shell_var_get(shell, shell->token + 1);

                if (variable == NULL) {
                    puts("print: Variable not found.\r\n"); return;
                }

                if (variable->type == SHELL_TYPE_INTEGER) {
                    char buffer[12];

                    itoa(*(uint16_t*)variable->value, buffer, 10);
                    puts(buffer);
                } else if (variable->type == SHELL_TYPE_BOOLEAN) {
                    puts(*(bool*)variable->value ? "true" : "false");
                } else if (variable->type == SHELL_TYPE_STRING) {
                    puts((char*)variable->value);
                }
            }
        } else {
            puts(shell->token);
        }

        putchar(' ');
    }

    puts("\r\n");
}

void shell_cmd_set(shell_t* shell) {
    if ((shell->token = strtok(NULL, " ")) == NULL) {
        puts("set: Variable name not specified.\r\n"); return;
    }

    char* name = shell->token;

    if (!isalpha(name)) {
        puts("set: Invalid variable name.\r\n"); return;
    }

    if ((shell->token = strtok(NULL, " ")) == NULL) {
        puts("set: Variable value not specified.\r\n"); return;
    }

    char* value = shell->token;

    if (isnumeric(value)) {
        uint16_t number = atoi(value);

        if (shell_var_set(shell, name, SHELL_TYPE_INTEGER, &number) == NULL)
            puts("set: Variable limit reached.\r\n");
    } else if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
        bool boolean = strcmp(value, "true") == 0;

        if (shell_var_set(shell, name, SHELL_TYPE_BOOLEAN, &boolean) == NULL)
            puts("set: Variable limit reached.\r\n");
    } else {
        shell_cmd_t* cmd = shell_cmd_find(value);

        if (cmd != NULL) {
            uint16_t output = 0;

            shell->token = value;
            shell->output = &output;

            cmd->func(shell);

            if (shell_var_set(shell, name, SHELL_TYPE_INTEGER, &output) == NULL)
                puts("set: Variable limit reached.\r\n");

            shell->output = NULL;
        } else {
            if (shell_var_set(shell, name, SHELL_TYPE_STRING, value) == NULL)
                puts("set: Variable limit reached.\r\n");
        }
    }
}

void shell_cmd_unset(shell_t* shell) {
    shell_var_unset(shell, strtok(NULL, " "));
}

void shell_cmd_run(shell_t* shell) {
    shell_line_sort(shell);

    for (size_t i = 0; i < 64; i++) {
        if (shell->script[i].index == 0)
            continue;

        shell_line_fix(shell, shell->script[i].index);

        shell->token = strtok(shell->script[i].buffer, " ");
        shell_cmd_t* cmd = shell_cmd_find(shell->token);

        if (cmd == NULL) {
            puts("run: Command not found.\r\n"); return; // TODO: print line number
        }

        cmd->func(shell);
    }
}

void shell_cmd_list(shell_t* shell) {
    shell_line_sort(shell);

    for (size_t i = 0; i < 64; i++) {
        if (shell->script[i].index == 0)
            continue;

        shell_line_fix(shell, shell->script[i].index);

        char buffer[6];

        itoa(shell->script[i].index, buffer, 10);
        puts(buffer);

        putchar(' ');

        puts(shell->script[i].buffer);
        puts("\r\n");
    }
}

void shell_cmd_eval(shell_t* shell) {
    uint16_t result = 0;

    char operator = '+';
    bool flag = true;

    while ((shell->token = strtok(NULL, " ")) != NULL) {
        if (flag) {
            uint16_t operand;

            if (shell->token[0] == '$') {
                shell_var_t* variable = shell_var_get(shell, shell->token + 1);

                if (variable == NULL) {
                    puts("eval: Variable not found.\r\n"); return;
                }

                if (variable->type != SHELL_TYPE_INTEGER) {
                    puts("eval: Variable is not an integer.\r\n"); return;
                }

                operand = *(uint16_t*)variable->value;
            } else if (isnumeric(shell->token)) {
                operand = atoi(shell->token);
            } else {
                puts("eval: Invalid operand.\r\n"); return;
            }

            switch (operator) {
                case '+': result += operand; break;
                case '-': result -= operand; break;
                case '*': result *= operand; break;
                case '/':
                    if (operand == 0) {
                        puts("eval: Division by zero.\r\n"); return;
                    }

                    result /= operand; break;

                default:
                    puts("eval: Invalid operator.\r\n"); return;
            }

            flag = false;
        } else {
            if (strlen(shell->token) != 1) {
                puts("eval: Invalid operator.\r\n"); return;
            }

            operator = shell->token[0];
            flag = true;
        }
    }

    if (shell->output != NULL) {
        *(uint16_t*)shell->output = result;
    } else {
        char buffer[12];

        itoa(result, buffer, 10);
        puts(buffer);

        puts("\r\n");
    }
}

void shell_cmd_peek(shell_t* shell) {
    uint32_t address;

    if ((shell->token = strtok(NULL, " ")) == NULL) {
        puts("peek: Address not specified.\r\n"); return;
    }

    if (shell->token[0] == '$') {
        shell_var_t* variable = shell_var_get(shell, shell->token + 1);

        if (variable == NULL) {
            puts("peek: Variable not found.\r\n"); return;
        }

        if (variable->type != SHELL_TYPE_INTEGER) {
            puts("peek: Variable is not an integer.\r\n"); return;
        }

        address = *(uint16_t*)variable->value;
    } else if (isnumeric(shell->token)) {
        address = atoi(shell->token);
    } else {
        puts("peek: Invalid address.\r\n"); return;
    }

    uint16_t segment = address >> 4;
    uint16_t offset = address & 0xf;

    uint8_t value;

    __asm__ (
        "movw %1, %%ax\n"
        "movw %2, %%si\n"
        "movw %%ax, %%es\n"
        "movb %%es:(%%si), %0\n"
        : "=g" (value)
        : "g" (segment), "g" (offset)
        : "ax", "si", "es"
    );

    if (shell->output != NULL) {
        *(uint16_t*)shell->output = value;
    } else {
        char buffer[4];

        itoa(value, buffer, 10);
        puts(buffer);

        puts("\r\n");
    }
}

void shell_cmd_poke(shell_t* shell) {
    uint32_t address;
    uint8_t value;

    if ((shell->token = strtok(NULL, " ")) == NULL) {
        puts("poke: Address not specified.\r\n"); return;
    }

    if (shell->token[0] == '$') {
        shell_var_t* variable = shell_var_get(shell, shell->token + 1);

        if (variable == NULL) {
            puts("poke: Variable not found.\r\n"); return;
        }

        if (variable->type != SHELL_TYPE_INTEGER) {
            puts("poke: Variable is not an integer.\r\n"); return;
        }

        address = *(uint16_t*)variable->value;
    } else if (isnumeric(shell->token)) {
        address = atoi(shell->token);
    } else {
        puts("poke: Invalid address.\r\n"); return;
    }

    if ((shell->token = strtok(NULL, " ")) == NULL) {
        puts("poke: Value not specified.\r\n"); return;
    }

    if (shell->token[0] == '$') {
        shell_var_t* variable = shell_var_get(shell, shell->token + 1);

        if (variable == NULL) {
            puts("poke: Variable not found.\r\n"); return;
        }

        if (variable->type != SHELL_TYPE_INTEGER) {
            puts("poke: Variable is not an integer.\r\n"); return;
        }

        value = *(uint16_t*)variable->value;
    } else if (isnumeric(shell->token)) {
        value = atoi(shell->token);
    } else {
        puts("poke: Invalid value.\r\n"); return;
    }

    uint16_t segment = address >> 4;
    uint16_t offset = address & 0xf;

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

void shell_cmd_int(shell_t* shell) {
    uint8_t interrupt;

    if ((shell->token = strtok(NULL, " ")) == NULL) {
        puts("int: Interrupt not specified.\r\n"); return;
    }

    char* names[6] = {"ax", "bx", "cx", "dx", "si", "di"};
    uint16_t registers[6] = {0};

    if (shell->token[0] == '$') {
        shell_var_t* variable = shell_var_get(shell, shell->token + 1);

        if (variable == NULL) {
            puts("int: Variable not found.\r\n"); return;
        }

        if (variable->type != SHELL_TYPE_INTEGER) {
            puts("int: Variable is not an integer.\r\n"); return;
        }

        interrupt = *(uint16_t*)variable->value;
    } else if (isnumeric(shell->token)) {
        interrupt = atoi(shell->token);
    } else {
        puts("int: Invalid interrupt value.\r\n"); return;
    }

    while ((shell->token = strtok(NULL, " ")) != NULL) {
        uint16_t value;

        if (shell->token[3] == '$') {
            shell_var_t* variable = shell_var_get(shell, shell->token + 4);

            if (variable == NULL) {
                puts("int: Variable not found.\r\n"); return;
            }

            if (variable->type != SHELL_TYPE_INTEGER) {
                puts("int: Variable is not an integer.\r\n"); return;
            }

            value = *(uint16_t*)variable->value;
        } else if (isnumeric(shell->token + 3)) {
            value = atoi(shell->token + 3);
        } else {
            puts("int: Invalid register value.\r\n"); return;
        }

        switch (shell->token[0]) {
            case 'a':
                switch (shell->token[1]) {
                    case 'x':
                        registers[0] = value; break;
                    case 'h':
                        registers[0] += value << 8; break;
                    case 'l':
                        registers[0] += value & 0xff; break;
                }

                break;
            case 'b':
                switch (shell->token[1]) {
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
                switch (shell->token[1]) {
                    case 'x':
                        registers[2] = value; break;
                    case 'h':
                        registers[2] += value << 8; break;
                    case 'l':
                        registers[2] += value & 0xff; break;
                }

                break;
            case 'd':
                switch (shell->token[1]) {
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
                switch (shell->token[1]) {
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

    if (shell->output == NULL) {
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

        puts("\r\n");
    }
}

void shell_cmd_read(shell_t* shell) {
    uint8_t drive;
    uint8_t amount;
    uint32_t position;
    uint32_t address;

    if ((shell->token = strtok(NULL, " ")) == NULL) {
        puts("read: Drive number not specified.\r\n"); return;
    }

    if (shell->token[0] == '$') {
        shell_var_t* variable = shell_var_get(shell, shell->token + 1);

        if (variable == NULL) {
            puts("read: Variable not found.\r\n"); return;
        }

        if (variable->type != SHELL_TYPE_INTEGER) {
            puts("read: Variable is not an integer.\r\n"); return;
        }

        drive = *(uint16_t*)variable->value;
    } else if (isnumeric(shell->token)) {
        drive = atoi(shell->token);
    } else {
        puts("read: Invalid drive number.\r\n"); return;
    }

    if ((shell->token = strtok(NULL, " ")) == NULL) {
        puts("read: Sector amount not specified.\r\n"); return;
    }

    if (shell->token[0] == '$') {
        shell_var_t* variable = shell_var_get(shell, shell->token + 1);

        if (variable == NULL) {
            puts("read: Variable not found.\r\n"); return;
        }

        if (variable->type != SHELL_TYPE_INTEGER) {
            puts("read: Variable is not an integer.\r\n"); return;
        }

        amount = *(uint16_t*)variable->value;
    } else if (isnumeric(shell->token)) {
        amount = atoi(shell->token);
    } else {
        puts("read: Invalid sector amount.\r\n"); return;
    }

    if ((shell->token = strtok(NULL, " ")) != NULL) {
        if (shell->token[0] == '$') {
            shell_var_t* variable = shell_var_get(shell, shell->token + 1);

            if (variable == NULL) {
                puts("read: Variable not found.\r\n"); return;
            }

            if (variable->type != SHELL_TYPE_INTEGER) {
                puts("read: Variable is not an integer.\r\n"); return;
            }

            position = *(uint16_t*)variable->value;
        } else if (isnumeric(shell->token)) {
            position = atoi(shell->token);
        } else {
            puts("read: Invalid position.\r\n"); return;
        }
    } else {
        position = 0;
    }

    if ((shell->token = strtok(NULL, " ")) != NULL) {
        if (shell->token[0] == '$') {
            shell_var_t* variable = shell_var_get(shell, shell->token + 1);

            if (variable == NULL) {
                puts("read: Variable not found.\r\n"); return;
            }

            if (variable->type != SHELL_TYPE_INTEGER) {
                puts("read: Variable is not an integer.\r\n"); return;
            }

            address = *(uint16_t*)variable->value;
        } else if (isnumeric(shell->token)) {
            address = atoi(shell->token);
        } else {
            puts("read: Invalid address.\r\n"); return;
        }
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

        itoa(status, shell->token, 16);
        puts(shell->token);

        puts(".\r\n");
    }
}

void shell_cmd_write(shell_t* shell) {
    uint8_t drive;
    uint8_t amount;
    uint32_t address;
    uint32_t position;

    if ((shell->token = strtok(NULL, " ")) == NULL) {
        puts("write: Drive number not specified.\r\n"); return;
    }

    if (shell->token[0] == '$') {
        shell_var_t* variable = shell_var_get(shell, shell->token + 1);

        if (variable == NULL) {
            puts("write: Variable not found.\r\n"); return;
        }

        if (variable->type != SHELL_TYPE_INTEGER) {
            puts("write: Variable is not an integer.\r\n"); return;
        }

        drive = *(uint16_t*)variable->value;
    } else if (isnumeric(shell->token)) {
        drive = atoi(shell->token);
    } else {
        puts("write: Invalid drive number.\r\n"); return;
    }

    if ((shell->token = strtok(NULL, " ")) == NULL) {
        puts("write: Sector amount not specified.\r\n"); return;
    }

    if (shell->token[0] == '$') {
        shell_var_t* variable = shell_var_get(shell, shell->token + 1);

        if (variable == NULL) {
            puts("write: Variable not found.\r\n"); return;
        }

        if (variable->type != SHELL_TYPE_INTEGER) {
            puts("write: Variable is not an integer.\r\n"); return;
        }

        amount = *(uint16_t*)variable->value;
    } else if (isnumeric(shell->token)) {
        amount = atoi(shell->token);
    } else {
        puts("write: Invalid sector amount.\r\n"); return;
    }

    if ((shell->token = strtok(NULL, " ")) != NULL) {
        if (shell->token[0] == '$') {
            shell_var_t* variable = shell_var_get(shell, shell->token + 1);

            if (variable == NULL) {
                puts("write: Variable not found.\r\n"); return;
            }

            if (variable->type != SHELL_TYPE_INTEGER) {
                puts("write: Variable is not an integer.\r\n"); return;
            }

            address = *(uint16_t*)variable->value;
        } else if (isnumeric(shell->token)) {
            address = atoi(shell->token);
        } else {
            puts("write: Invalid address.\r\n"); return;
        }
    } else {
        address = 0x7e00;
    }

    if ((shell->token = strtok(NULL, " ")) != NULL) {
        if (shell->token[0] == '$') {
            shell_var_t* variable = shell_var_get(shell, shell->token + 1);

            if (variable == NULL) {
                puts("write: Variable not found.\r\n"); return;
            }

            if (variable->type != SHELL_TYPE_INTEGER) {
                puts("write: Variable is not an integer.\r\n"); return;
            }

            position = *(uint16_t*)variable->value;
        } else if (isnumeric(shell->token)) {
            position = atoi(shell->token);
        } else {
            puts("write: Invalid position.\r\n"); return;
        }
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

        itoa(status, shell->token, 16);
        puts(shell->token);

        puts(".\r\n");
    }
}

shell_cmd_t shell_cmds[] = {
    {"print", shell_cmd_print},
    {"clear", clear},
    {"set", shell_cmd_set},
    {"unset", shell_cmd_unset},
    {"run", shell_cmd_run},
    {"list", shell_cmd_list},
    {"eval", shell_cmd_eval},
    {"peek", shell_cmd_peek},
    {"poke", shell_cmd_poke},
    {"int", shell_cmd_int},
    {"read", shell_cmd_read},
    {"write", shell_cmd_write},
    {NULL, NULL}
};