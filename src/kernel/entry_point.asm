[bits 16] ; 16-bit mode
[extern main] ; main kernel function

global _start ; set the entry point

_start:
    call main ; call the main function

    ; shut down the system in case the kernel returns
    cli ; disable interrupts
    hlt ; halt the CPU