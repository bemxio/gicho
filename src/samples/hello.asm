[bits 16] ; 16-bit mode
[org 0x7e00] ; global offset

mov si, HELLO_WORLD ; load the address of the string
call print ; print the string

ret ; return from the program

print:
    pusha ; save registers

    mov ah, 0x0e ; 'Teletype Output' function
    xor bh, bh ; set page number to 0

    print_loop:
        mov al, [si] ; move the character from the source index to the register

        test al, al ; check if the character is null
        jz print_end ; if so, print is done

        int 0x10 ; call the BIOS interrupt

        inc si ; move to the next character
        jmp print_loop ; repeat

    print_end:
        popa ; restore registers
        ret ; return from function

; constants
HELLO_WORLD db "Hello, world!", 0x0d, 0x0a, 0x00 ; the string