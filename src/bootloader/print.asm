print:
    pusha ; save registers

    mov ah, 0x0e ; "Teletype Output" function
    xor bh, bh ; reset page number

    print_loop:
        mov al, [si] ; move character from source index to register

        test al, al ; check if character is null
        jz print_end ; jump to end if true

        int 0x10 ; BIOS interrupt

        inc si ; increment source index
        jmp print_loop ; repeat

    print_end:
        popa ; restore registers
        ret ; return from function

print_hex:
    pusha ; save registers

    mov ch, cl ; copy original value to register
    shr ch, 0x04 ; shift right by 4 bits
    call print_hex_digit ; print high nibble

    mov ch, cl ; copy original value to register
    and ch, 0x0f ; mask low 4 bits
    call print_hex_digit ; print low nibble

    popa ; restore registers
    ret ; return from function

    print_hex_digit:
        add ch, "0" ; convert value to ASCII

        cmp ch, "9" ; check if value is less than or equal to 9
        jle print_hex_char ; print character if true

        add ch, 0x07 ; adjust value to correct ASCII character

    print_hex_char:
        mov al, ch ; move character to register
        mov ah, 0x0e ; "Teletype Output" function

        int 0x10 ; BIOS interrupt

        ret ; return from function