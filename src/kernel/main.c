#include "shell.h"

void kmain() {
    char buffer[256];

    clear();

    for (;;) {
        gets(buffer);
        shell_execute_cmd(buffer);
    }
}