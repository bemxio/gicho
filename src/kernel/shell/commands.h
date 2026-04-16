#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#include "../lib/io.h"
#include "../lib/string.h"

typedef struct {
    char* name;
    void (*func)(char* token);
} shell_cmd_t;

shell_cmd_t shell_cmds[];

#endif