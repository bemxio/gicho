#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VARIABLE_COUNT 32
#define LINE_COUNT 64

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
    uint8_t length;
    char* buffer;
} shell_line_t;

typedef struct shell_t {
    char* token;
    shell_var_t variables[VARIABLE_COUNT];
    shell_line_t script[LINE_COUNT];
    void* output;
    bool in_script;
    size_t line_index;
} shell_t;

shell_var_t* shell_var_get(shell_t* shell, char* name);
shell_var_t* shell_var_set(shell_t* shell, char* name, shell_type_t type, void* value);
void shell_var_unset(shell_t* shell, char* name);

shell_line_t* shell_line_get(shell_t* shell, uint16_t index);
shell_line_t* shell_line_set(shell_t* shell, uint16_t index, char* buffer);
void shell_line_unset(shell_t* shell, uint16_t index);
void shell_line_sort(shell_t* shell);
void shell_line_fix(shell_t* shell, uint16_t index);

#endif