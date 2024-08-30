disk_read:
    pusha ; save registers

    mov ah, 0x02 ; 'Read Sectors Into Memory' function
    mov al, 0x01 ; sector amount

    mov cl, 0x02 ; sector (0x02 is the first 'available' sector)
    mov ch, 0x00 ; cylinder (0x0 - 0x3FF)
    mov dh, 0x00 ; head number (0x0 - 0xF)

    int 0x13 ; call the BIOS interrupt
    jc disk_error ; if the carry flag is set, there was an error

    popa ; restore registers
    ret ; return from function

disk_error:
    mov si, DISK_ERROR_MESSAGE ; load the address of the error message
    mov cl, ah ; load the error code into the `cl` register

    call print ; print the error message
    call print_hex ; print the error code in hex

    hlt ; halt the system