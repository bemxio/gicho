#include "lib/io.h"
#include "lib/string.h"
#include "shell/shell.h"

void kmain() {
    char buffer[256];
    shell_t shell;

    clear();

    for (;;) {
        gets(buffer);
        shell_execute_cmd(&shell, buffer);
    }
}