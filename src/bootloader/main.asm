[bits 16] ; 16-bit mode
[org 0x7c00] ; global offset

mov si, START_MESSAGE ; set the address of the start message
call print ; print the start message

mov bx, 0x0500 ; set the address for the kernel

call disk_read ; read the kernel code
jmp bx ; jump to the kernel

; includes
%include "src/bootloader/print.asm"
%include "src/bootloader/disk.asm"

; strings
START_MESSAGE db "Starting Gicho...", 0x0d, 0x0a, 0x00
DISK_ERROR_MESSAGE db "Error: Disk read failed with error code 0x", 0x00
LINE_BREAK db 0x0d, 0x0a, 0x00

; pad the rest of the sector with zeros
times 510 - ($ - $$) db 0x00

; boot signature
dw 0xaa55