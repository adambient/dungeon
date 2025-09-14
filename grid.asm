MAP_SIZE: equ $10 ; 64
COMPRESSED_MAP_WIDTH: equ $08 ; half of MAP_SIZE

SECTION code_user

PUBLIC _grid_get
PUBLIC _grid_set

;----------
; _grid_get
; inputs: grid
; outputs: hl = grid value
;----------
_grid_get:
            ld b, (hl) ; b = x
            inc hl
            ld c, (hl) ; c = y
            call load_cell_location            
            ld a, (hl) ; load 8 bit value into a
            bit $00, c ; is x even?
            jr z, grid_get_end
            or a ; clear carry so doesn't get rotated into number
            rra
            rra
            rra
            rra ; rotate the last 4 bits to the first 4
grid_get_end:
            and $0f ; blank out the last 4 bits
            ld h, $00
            ld l, a ; hl = grid value
            ret

;----------
; _grid_set:
; inputs: grid
;----------
_grid_set:
            ld b, (hl) ; b = x
            inc hl
            ld c, (hl) ; c = y
            inc hl
            ld a, (hl) ; a = grid value             
            ex af, af' ; store a
            call load_cell_location ; load cell location bc into hl
            ex af, af' ; retrieve a            
            bit $00, c ; is x even?
            jr z, grid_set_even
            or a ; clear carry so doesn't get rotated into number
            rla ; x not even
            rla
            rla
            rla ; rotate the first 4 bits to the last 4
            ld e, a ; e = given value on rhs
            ld a, (hl)            
            and $0f ; a = current lhs value
            jr grid_set_end
grid_set_even: ; x is even
            and $0f ; blank out the last 4 bits so we don't overwrite
            ld e, a ; e = given value on lhs
            ld a, (hl)            
            and $f0 ; a = current rhs value
grid_set_end:
            or e ; a = combined given and current value
            ld (hl), a ; store back in location
            ret

;----------
; load_cell_location
; inputs: b = x, c = y
; outputs: hl = cell location within grid
; alters: a, b, de, hl
;----------
load_cell_location:            
            ld a, b ; load a with x
            ld hl, _map ; point hl at _map
            ld d, $00 
            ld e, a ; de = a
            add hl, de ; hl = _map + x
            ;ld d, $00 - already 0
            ld e, c ; de = y           
            ex de, hl ; hl = y
            add hl, hl
            ;add hl, hl
            ;add hl, hl
            add hl, hl
            add hl, hl; hl = y * COMPRESSED_MAP_WIDTH(=32)
            add hl, de ; hl = _map + x + (y * COMPRESSED_MAP_WIDTH)
            ret

SECTION bss_user

PUBLIC _grid

_grid:
db %00000000 ;x
db %00000000 ;y
db %00000000 ;tile

_map: ds MAP_SIZE*(COMPRESSED_MAP_WIDTH + 1)