disk_read:
    pusha ; save registers

    mov ah, 0x02 ; 'Read Sectors Into Memory' function
    mov al, KERNEL_SIZE ; sector amount

    mov cl, 0x02 ; sector (0x02 is the first 'available' sector)
    mov ch, 0x00 ; cylinder (0x0 - 0x3FF)
    mov dh, 0x00 ; head number (0x0 - 0xF)

    int 0x13 ; BIOS interrupt
    jc disk_error ; handle error if carry flag is set

    popa ; restore registers
    ret ; return from function

disk_error:
    mov si, DISK_ERROR_MESSAGE ; set string index to message address
    mov cl, ah ; set error code

    call print ; print error message
    call print_hex ; print error code in hex

    cli ; disable interrupts
    hlt ; halt CPU