[bits 16] ; 16-bit mode
[extern main] ; main kernel function

global _start ; set entry point

_start:
    call main ; call main function

    cli ; disable interrupts
    hlt ; halt CPU