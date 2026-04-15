#include "io.h"
#include "commands.h"

#include <stdint.h>

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
            }
        }
    }
}