#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#include "io.h"
#include "string.h"

typedef struct {
    char* name;
    void (*function)(char* token);
} command_t;

command_t commands[];

#endif