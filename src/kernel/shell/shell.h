#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>

typedef enum {
    SHELL_TYPE_INTEGER,
    SHELL_TYPE_FLOAT,
    SHELL_TYPE_STRING,
    SHELL_TYPE_BOOLEAN
} shell_type_t;

typedef struct {
    char* name;
    shell_type_t type;
    void* value;
} shell_var_t;

typedef struct shell_t {
    char* token;
    shell_var_t variables[32];
} shell_t;

shell_var_t* shell_var_get(shell_t* shell, char* name);
shell_var_t* shell_var_set(shell_t* shell, char* name, shell_type_t type, void* value);

void shell_execute_cmd(shell_t* shell, char* input);

#endif