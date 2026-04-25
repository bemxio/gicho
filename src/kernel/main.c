#include "lib/int.h"
#include "lib/io.h"
#include "lib/memory.h"
#include "lib/string.h"

#include "shell/commands.h"
#include "shell/shell.h"

void kmain() {
    shell_t shell;
    char buffer[256];
    bool in_script = false;

    //shell_var_unset(&shell, NULL);
    shell_line_unset(&shell, 0);

    shell.output = NULL;

    uint16_t bytes_free = 0xffff - KERNEL_SIZE * 512 - 0x500;
    itoa(bytes_free, buffer, 10);

    clear();

    puts("Gicho v2.0\r\n");
    puts(buffer);
    puts(" bytes free.\r\n");

    for (;;) {
        if (!in_script)
            puts("\r\nReady.\r\n");

        gets(buffer);

        shell.token = strtok(buffer, " ");

        if (shell.token == NULL || shell.token[0] == '\0')
            continue;

        if (isnumeric(shell.token)) {
            uint16_t index = atoi(shell.token);

            if (index == 0) {
                puts("Invalid line number.\r\n"); continue;
            }

            if (!shell_line_set(&shell, index, shell.token + strlen(shell.token) + 1))
                puts("Script size limit reached.\r\n");

            in_script = true;
        } else {
            shell_cmd_t* cmd = shell_cmd_find(shell.token);

            if (cmd == NULL)
                puts("Command not found.\r\n");
            else
                cmd->func(&shell);

            in_script = false;
        }
    }
}