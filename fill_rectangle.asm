VIDEOATT: equ $5800 ; address of attribute RAM
VIDEOATT_L: equ $0300 ; length of attribute RAM
ATTR_BUFF: equ $8000 ; hard coded attribute buffer address to start of uncontended memory, just before CRT_ORG_CODE ($8300)

SECTION code_user

PUBLIC _fill_rectangle_char
PUBLIC _fill_rectangle_attr
PUBLIC _bright_rectangle_attr
PUBLIC _copy_attr_buffer

;----------
; _fill_rectangle_char
; inputs: fill_rectangle_p
;----------
_fill_rectangle_char:
            ld c, (hl) ; c = y
            inc hl
            ld e, (hl) ; e = x
            ld hl, (_fill_rectangle_param+2) ; h = height, l = width            
            ld ix, (_fill_rectangle_param+6) ; ix = address of first char
            ld b, h ; set counter 1 to height
_fill_rectangle_char_loop1:
            ld d, c ; retrieve initial y
            push bc ; store counter 1
            ld b, l ; set counter 2 to width
            ;----------
            ; BEGIN - get_char_address (inline) - adapted from a routine by Dean Belfield
            ; inputs: d = y, e = x
            ; outputs: hl = location of screen address
            ;---------
            ld a,e
            and $07
            rra
            rra
            rra
            rra
            or d
            ld l,a
            ld a,e
            and $18
            or $40
            ld h,a
            ; END
_fill_rectangle_char_loop2:
            ld a, (ix)
            push hl ; store hl = screen address
            exx ; store hl = screen address, bc = counter 2, de = xy
            pop hl ; retrieve hl = screen address                                 
            cp '$' ; means end of string
            jr nz, _fill_rectangle_char_print_char ; if not null goto print            
            ld ix, (_fill_rectangle_param+6)
            ld a, (ix) ; reset string 
_fill_rectangle_char_print_char:
            ; get glyph from char
            ld de, udgs
            add a, a
            add a, a
            add a, a
            push hl
            ld l, a
            ld h, 0
            add hl, de
            ex de, hl
            pop hl
            ld b, 8 ; loop counter
_fill_rectangle_char_loop3:
            ld a, (de) ; get the byte
            ld (hl), a ; print to screen
            inc de ; goto next byte of character
            inc h ; goto next line of screen
            djnz _fill_rectangle_char_loop3 ; loop 8 times
            exx ; retrieve de = xy, bc = counter 2, hl = screen address
            inc hl ; goto next character
            inc d ; increase y
            inc ix ; increase char
            djnz _fill_rectangle_char_loop2                         
            pop bc ; retrieve counter 1
            ld hl, (_fill_rectangle_param+2) ; h = height, l = width
            inc e ; increase x
            djnz _fill_rectangle_char_loop1  
            ret

;----------
; BEGIN - get_attr_address - adapted from a routine by Jonathan Cauldwell
; inputs: d = y, e = x
; outputs: hl = location of attribute (buffer) address
;----------
get_attr_address:
            ld bc, ATTR_BUFF            
            ld a,e
            rrca
            rrca
            rrca
            ld l,a
            and $03            
            add a, b ; add hi byte of ATT_BUFF address
            ld h,a
            ld a,l
            and $e0
            ld l,a
            ld a,d
            add a, l
            ld l,a
            ret

;----------
; _fill_rectangle_attr
; inputs: fill_rectangle_p
;----------
_fill_rectangle_attr:
            ld c, (hl) ; c = y
            inc hl
            ld e, (hl) ; e = x
            ld hl, (_fill_rectangle_param+2) ; h = height, l = width
            ld b, h ; set counter 1 to height
_fill_rectangle_attr_loop1:
            ld d, c ; retrieve initial y
            push bc ; store counter 1
            ld b, l ; set counter 2 to width                        
            push bc ; store counter 2
            call get_attr_address
            pop bc ; retrieve counter 2
_fill_rectangle_attr_loop2:
            push bc ; store counter 2
            ld bc, (_fill_rectangle_param+4) ; retrive paper/ink
            ld a, c; load paper
            rla 
            rla
            rla ; paper in position
            or b ; add ink
            ld (hl), a ; store in location
            inc hl ; go to next location
            pop bc ; retrieve counter 2
            inc d ; increase y
            djnz _fill_rectangle_attr_loop2                         
            pop bc ; retrieve counter 1
            ld hl, (_fill_rectangle_param+2) ; h = height, l = width
            inc e ; increase x
            djnz _fill_rectangle_attr_loop1  
            ret

;----------
; _bright_rectangle_attr
; inputs: fill_rectangle_p
;----------
_bright_rectangle_attr:
            ld c, (hl) ; c = y
            inc hl
            ld e, (hl) ; e = x
            ld hl, (_fill_rectangle_param+2) ; h = height, l = width
            ld b, h ; set counter 1 to height
_bright_rectangle_attr_loop1:
            ld d, c ; retrieve initial y
            push bc ; store counter 1
            ld b, l ; set counter 2 to width                        
            push bc ; store counter 2
            call get_attr_address
            pop bc ; retrieve counter 2
_bright_rectangle_attr_loop2:
            ld a, (hl)
            or %01000000
            ld (hl), a ; store in location
            inc hl ; go to next location
            inc d ; increase y
            djnz _bright_rectangle_attr_loop2                         
            pop bc ; retrieve counter 1
            ld hl, (_fill_rectangle_param+2) ; h = height, l = width
            inc e ; increase x
            djnz _bright_rectangle_attr_loop1  
            ret

;----------
; copy_attr_buffer
; copy attribute buffer into attribute memory
; alters: hl, de, bc
;----------
_copy_attr_buffer:
            ld de, VIDEOATT ; target is attribute memory
            ld hl, ATTR_BUFF ; source is attribute buffer
            ld bc, VIDEOATT_L ; length is size of attribute memory
            ldir ; copy
            ret

SECTION bss_user
PUBLIC _fill_rectangle_param
_fill_rectangle_param:
db %00000000 ;y
db %00000000 ;x
db %00000000 ;width 
db %00000000 ;height
db %00000000 ;paper
db %00000000 ;ink
dw $0000 ;char

SECTION rodata_user
udgs: 
defb 0,0,0,0,0,0,0,0 ; @ - space
defb 126,129,189,189,129,189,165,231 ; A
defb 254,131,189,131,189,189,129,254 ; B
defb 126,195,189,167,167,189,195,126 ; C
defb 252,134,187,165,165,187,134,252 ; D
defb 126,195,189,135,191,189,195,126 ; E
defb 255,129,191,130,190,160,160,224 ; F
defb 126,195,189,191,177,189,195,126 ; G
defb 231,165,189,129,189,165,165,231 ; H
defb 127,65,119,20,20,119,65,127 ; I
defb 7,5,5,229,165,189,195,126 ; J
defb @11111111 ; K - man up top left 1
defb @11111100
defb @11111000
defb @11111000
defb @11110000
defb @11110000
defb @11111000
defb @11111100
defb @11111111 ; L - man up top right 1
defb @00111111
defb @00011111
defb @00011111
defb @00001111
defb @00001111
defb @00011111
defb @00111111
defb @11111111 ; M - man down top left 1
defb @11111100
defb @11111000
defb @11111000
defb @11110010
defb @11110010
defb @11111000
defb @11111100
defb @11111111 ; N - man down top right 1
defb @00111111
defb @00011111
defb @00011111
defb @01001111
defb @01001111
defb @00011111
defb @00111111
defb @11111011 ; O - man right bottom left 1
defb @11110010
defb @11110010
defb @11111010
defb @11111110
defb @11111111
defb @11111000
defb @11111111
defb @11011111 ; P - man right bottom right 1
defb @01001111
defb @01001111
defb @01011111
defb @01111111
defb @11111111
defb @00011111
defb @11111111
defb @11111111 ; Q - man right bottom left 2
defb @11111100
defb @11111100
defb @11111100
defb @11110100
defb @11110111
defb @11110100
defb @11111111
defb @11011111 ; R - man right bottom right 2
defb @10001111
defb @10001111
defb @10011111
defb @11111111
defb @10011111
defb @00000111
defb @11111111
defb @11111011 ; S - man right bottom left 3
defb @11110001
defb @11110001
defb @11111001
defb @11111111
defb @11111001
defb @11100000
defb @11111111
defb @11111111 ; T - man right bottom right 3
defb @00111111
defb @00111111
defb @00111111
defb @00101111
defb @11101111
defb @00101111
defb @11111111
defb @11101111 ; U - man down bottom left 1
defb @11011011
defb @10010000
defb @10010000
defb @11111000
defb @11111111
defb @11110001
defb @11111111
defb @11110111 ; V - man down bottom right 1
defb @11011011
defb @00001001
defb @00001001
defb @00011111
defb @11111111
defb @10001111
defb @11111111
defb @11101111 ; W - man down bottom left 2
defb @11011011
defb @11001000
defb @11001000
defb @11111000
defb @11111111
defb @11111111
defb @11111111
defb @11110111 ; X - man down bottom right 2
defb @11011011
defb @00001011
defb @00001011
defb @00011111
defb @11111111
defb @00001111
defb @11111111
defb @11101111 ; Y - man down bottom left 3
defb @11011011
defb @11010000
defb @11010000
defb @11111000
defb @11111111
defb @11110000
defb @11111111
defb @11110111 ; Z - man down bottom right 3
defb @11011011
defb @00010011
defb @00010011
defb @00011111
defb @11111111
defb @11111111
defb @11111111
defb 255,255,255,255,0,0,0,0 ; [ - horizontal stripe
defb %11110000
defb %11110000
defb %11110000
defb %11110000
defb %11110000
defb %11110000
defb %11110000
defb %11110000 ; \ - vertical stripe
