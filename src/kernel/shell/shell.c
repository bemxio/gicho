#include "shell.h"

void shell_execute_cmd(char* input) {
    char* token = strtok(input, " ");

    if (token == NULL || token[0] == '\0') {
        return;
    }

    for (shell_cmd_t* cmd = shell_cmds; cmd->name != NULL; cmd++) {
        if (strcmp(token, cmd->name) == 0) {
            cmd->func(token); return;
        }
    }

    puts("Command not found.\r\n");
}