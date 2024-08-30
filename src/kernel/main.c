int kernel_main() {
    const char* text = "Hello, world!";

    for (int i = 0; text[i] != '\0'; i++) {
        __asm__ (
            "mov $0x0e, %%ah\n"
            "mov $0x0000, %%bx\n"
            "mov %0, %%al\n"
            "int $0x10\n"
            :: "r" (text[i])
        );
    }

    return 0;
}