#include "commands.h"
#include "io.h"

void kmain() {
    char buffer[256];
    char* token;

    clear();

    for (;;) {
        gets(buffer);

        if ((token = strtok(buffer, " ")) == NULL || token[0] == '\0')
            continue;

        for (size_t i = 0; commands[i].name != NULL; i++) {
            if (strcmp(token, commands[i].name) == 0) {
                commands[i].function(token); break;
            } else if (commands[i + 1].name == NULL) {
                puts("Command not found.\r\n");
            }
        }
    }
}