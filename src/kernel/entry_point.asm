[bits 16] ; 16-bit mode
[extern kmain] ; main kernel function

global _start ; set the entry point

_start:
    call kmain ; call the main function
    jmp $ ; infinite loop in case the function returns