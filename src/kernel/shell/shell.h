#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    SHELL_TYPE_INTEGER,
    //SHELL_TYPE_FLOAT,
    SHELL_TYPE_STRING,
    SHELL_TYPE_BOOLEAN
} shell_type_t;

typedef struct {
    char* name;
    shell_type_t type;
    void* value;
} shell_var_t;

typedef struct {
    uint16_t index;
    char* buffer;
} shell_line_t;

typedef struct shell_t {
    char* token;
    shell_var_t variables[32];
    shell_line_t script[64];
    void* output;
} shell_t;

shell_var_t* shell_var_get(shell_t* shell, char* name);
shell_var_t* shell_var_set(shell_t* shell, char* name, shell_type_t type, void* value);
void shell_var_unset(shell_t* shell, char* name);

shell_line_t* shell_line_get(shell_t* shell, uint16_t index);
shell_line_t* shell_line_set(shell_t* shell, uint16_t index, char* buffer);
void shell_line_unset(shell_t* shell, uint16_t index);
void shell_line_sort(shell_t* shell);

#endif