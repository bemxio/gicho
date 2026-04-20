#include "lib/io.h"
#include "lib/string.h"
#include "shell/shell.h"

#define KERNEL_SIZE *((uint16_t*)0xffff) // kernel size in sectors

void kmain() {
    shell_t shell;
    char buffer[256];

    uint16_t bytes_free = 0xffff - KERNEL_SIZE * 512 - 0x500;
    itoa(bytes_free, buffer, 10);

    clear();

    puts("Gicho v2.0\r\n");
    puts(buffer);
    puts(" bytes free.\r\n");

    for (;;) {
        puts("\r\nReady.\r\n");
        gets(buffer);
        shell_execute_cmd(&shell, buffer);
    }
}