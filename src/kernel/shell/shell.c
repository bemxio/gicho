#include "shell.h"

#include "../lib/io.h"
#include "../lib/string.h"
#include "commands.h"

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