[bits 16] ; 16-bit mode
[extern kernel_main] ; main kernel function

global _start ; set the entry point

_start:
    call kernel_main ; call the main function
    jmp $ ; infinite loop in case the function returns