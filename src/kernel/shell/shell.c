#include "shell.h"

#include "../lib/io.h"
#include "../lib/memory.h"
#include "../lib/string.h"
#include "commands.h"

#include <stddef.h>

// variable functions
shell_var_t* shell_var_new(shell_t* shell) {
    for (size_t i = 0; i < 32; i++) {
        if (shell->variables[i].name == NULL)
            return &shell->variables[i];
    }

    return NULL;
}

shell_var_t* shell_var_get(shell_t* shell, char* name) {
    for (size_t i = 0; i < 32; i++) {
        if (shell->variables[i].name == NULL)
            continue;

        if (strcmp(shell->variables[i].name, name) != 0)
            continue;

        return &shell->variables[i];
    }

    return NULL;
}

shell_var_t* shell_var_set(shell_t* shell, char* name, shell_type_t type, void* value) {
    shell_var_t* variable = shell_var_get(shell, name);

    if (variable == NULL)
        variable = shell_var_new(shell);

    if (variable == NULL)
        return NULL;

    if (variable->name == NULL) {
        variable->name = malloc(strlen(name) + 1);
        strcpy(variable->name, name);
    }

    variable->type = type;

    switch (type) {
        case SHELL_TYPE_INTEGER:
            variable->value = malloc(sizeof(uint16_t));
            *(uint16_t*)variable->value = *(uint16_t*)value;

            break;

        case SHELL_TYPE_BOOLEAN:
            variable->value = malloc(sizeof(bool));
            *(bool*)variable->value = *(bool*)value;

            break;

        case SHELL_TYPE_STRING:
            variable->value = malloc(strlen((char*)value) + 1);
            strcpy(variable->value, (char*)value);

            break;
    }

    return variable;
}

void shell_var_unset(shell_t* shell, char* name) {
    if (name == NULL) {
        for (size_t i = 0; i < 32; i++) {
            shell->variables[i].name = NULL;
            shell->variables[i].value = NULL;
        }

        return;
    }

    shell_var_t* variable = shell_var_get(shell, name);

    if (variable == NULL)
        return;

    variable->name = NULL;
    variable->value = NULL;
}

// script functions
shell_line_t* shell_line_new(shell_t* shell) {
    for (size_t i = 0; i < 64; i++) {
        if (shell->script[i].index == 0 && shell->script[i].buffer == NULL)
            return &shell->script[i];
    }

    return NULL;
}

shell_line_t* shell_line_get(shell_t* shell, uint16_t index) {
    for (size_t i = 0; i < 64; i++) {
        if (shell->script[i].index == 0 || shell->script[i].index != index)
            continue;

        return &shell->script[i];
    }

    return NULL;
}

shell_line_t* shell_line_set(shell_t* shell, uint16_t index, char* buffer) {
    shell_line_t* line = shell_line_get(shell, index);

    if (line == NULL)
        line = shell_line_new(shell);

    if (line == NULL)
        return NULL;

    line->index = index;
    line->length = strlen(buffer);
    line->buffer = malloc(line->length + 1);

    strcpy(line->buffer, buffer);

    return line;
}

void shell_line_unset(shell_t* shell, uint16_t index) {
    if (index == 0) {
        for (size_t i = 0; i < 64; i++) {
            shell->script[i].index = 0;
            shell->script[i].buffer = NULL;
        }

        return;
    }

    shell_line_t* line = shell_line_get(shell, index);

    if (line == NULL)
        return;

    line->index = 0;
    line->buffer = NULL;
}

void shell_line_sort(shell_t* shell) {
    for (size_t i = 0; i < 64; i++) {
        for (size_t j = 0; j < 63 - i; j++) {
            if (shell->script[j].index == 0 || shell->script[j + 1].index == 0)
                continue;

            if (shell->script[j].index <= shell->script[j + 1].index)
                continue;

            shell_line_t temp = shell->script[j];

            shell->script[j] = shell->script[j + 1];
            shell->script[j + 1] = temp;
        }
    }
}

void shell_line_fix(shell_t* shell, uint16_t index) {
    shell_line_t* line = shell_line_get(shell, index);

    if (line == NULL)
        return;

    for (size_t i = 0; i < line->length; i++) {
        if (line->buffer[i] == '\0')
            line->buffer[i] = ' ';
    }
}