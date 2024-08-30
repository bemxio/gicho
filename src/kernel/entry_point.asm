[bits 16] ; 16-bit mode
[extern main] ; main kernel function

global _start ; set the entry point

_start:
    call main ; call the main function
    jmp $ ; infinite loop in case the function returns