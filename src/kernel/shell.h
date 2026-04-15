#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

#include "io.h"
#include "string.h"

typedef struct {
    char* name;
    void (*func)(char* token);
} shell_cmd_t;

shell_cmd_t shell_cmds[];

void shell_execute_cmd(char* input);

#endif