[bits 16] ; 16-bit mode
[org 0x7c00] ; global offset

jmp $ ; infinite loop

; pad the rest of the sector with zeros
times 510 - ($ - $$) db 0x00

; boot signature
dw 0xaa55