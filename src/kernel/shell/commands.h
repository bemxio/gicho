#ifndef COMMANDS_H
#define COMMANDS_H

#include "shell.h"

typedef struct {
    char* name;
    void (*func)(shell_t* shell);
} shell_cmd_t;

shell_cmd_t shell_cmds[];

#endif