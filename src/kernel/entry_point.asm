[bits 16] ; 16-bit mode
[extern kmain] ; main kernel function

global _start ; set entry point

_start:
    call kmain ; call main kernel function

    cli ; disable interrupts
    hlt ; halt CPU