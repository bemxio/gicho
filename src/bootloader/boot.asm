[bits 16] ; 16-bit mode
[org 0x7c00] ; global offset

mov si, BOOT_MESSAGE ; set string index to message address
call print ; print boot message

mov bx, 0x0500 ; set kernel location
call disk_read ; read kernel code from disk

mov [0xfffe], dl ; store drive number
mov [0xffff], KERNEL_SIZE ; store kernel size in sectors

jmp bx ; jump to kernel code

; includes
%include "src/bootloader/print.asm"
%include "src/bootloader/disk.asm"

; strings
BOOT_MESSAGE db "Starting Gicho...", 0x0d, 0x0a, 0x00
DISK_ERROR_MESSAGE db "Error: Disk read failed with error code 0x", 0x00

; sector padding
times 510 - ($ - $$) db 0x00

; boot signature
dw 0xaa55