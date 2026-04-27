#ifndef COMMANDS_H
#define COMMANDS_H

#include "shell.h"

typedef struct {
    char* name; // command name
    void (*func)(shell_t* shell); // pointer to command function
} shell_cmd_t; // command

shell_cmd_t shell_cmds[]; // list of commands, defined in commands.c
shell_cmd_t* shell_cmd_find(char* name); // finds a command by name, returns NULL if not found, case-insensitive

#endif