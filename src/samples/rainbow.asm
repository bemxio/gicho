[bits 16] ; 16-bit mode
[org 0x7e00] ; global offset

mov ah, 0x00 ; 'Set Video Mode' function
mov al, 0x13 ; 320x200x256 color mode (VGA)

int 0x10 ; call the BIOS interrupt

mov dx, INITIAL_ROW ; set the initial row counter
mov cx, INITIAL_COLUMN ; set the initial column counter

main_loop:
    call draw_line ; draw a line

    inc dx ; increment the row counter

    cmp dx, SCREEN_HEIGHT ; compare the row counter to the screen height
    jne main_loop ; if the row counter is not equal, loop back to the beginning

ret ; return from the program

draw_segment:
    xor bl, bl ; reset the iteration counter

    draw_segment_loop:
        mov ah, 0x0c ; 'Write Graphics Pixel' function
        int 0x10 ; call the BIOS interrupt

        inc cx ; increment the column counter
        inc bl ; increment the iteration counter

        cmp bl, SEGMENT_LENGTH ; compare the iteration counter to the segment length
        jne draw_segment_loop ; if the iteration counter is not equal, loop back to the beginning

    draw_segment_end:
        ret ; return from the function

draw_line:
    pusha ; save all of the registers to the stack

    mov cx, INITIAL_COLUMN ; set the column counter to the initial column
    mov al, INITIAL_COLOR ; set the color value to the initial color

    draw_line_loop:
        call draw_segment ; draw a color segment

        inc al ; increment the color value

        cmp cx, SCREEN_WIDTH ; compare the column counter to the screen width
        jne draw_line_loop ; if the column counter is not equal, loop back to the beginning

    draw_line_end:
        popa ; restore all of the registers from the stack
        ret ; return from the function

; constants
SCREEN_WIDTH equ 320 ; the width of the screen
SCREEN_HEIGHT equ 200 ; the height of the screen

INITIAL_ROW equ 0 ; the initial row value
INITIAL_COLUMN equ 0 ; the initial column value
INITIAL_COLOR equ 0x20 ; the initial color value

SEGMENT_LENGTH equ 20 ; the length of each color segment