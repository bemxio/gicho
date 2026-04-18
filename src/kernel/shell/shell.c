#include "shell.h"

#include "../lib/io.h"
#include "../lib/mem.h"
#include "../lib/string.h"
#include "commands.h"

#include <stddef.h>

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
            variable->value = malloc(sizeof(int));
            *(int*)variable->value = *(int*)value;

            break;

        /*
        case SHELL_TYPE_FLOAT:
            variable->value = malloc(sizeof(float));
            *(float*)variable->value = *(float*)value;

            break;
        */

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

void shell_execute_cmd(shell_t* shell, char* input) {
    shell->token = strtok(input, " ");

    if (shell->token == NULL || shell->token[0] == '\0') {
        return;
    }

    for (shell_cmd_t* cmd = shell_cmds; cmd->name != NULL; cmd++) {
        if (strcmp(shell->token, cmd->name) == 0) {
            cmd->func(shell); return;
        }
    }

    puts("Command not found.\r\n");
}