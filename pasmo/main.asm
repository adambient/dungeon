include "pasmo\consts.asm"

org 24500
; initialize ay mixer to all notes but no noises
ld a, 7
ld h, %00111000
ld c, $fd
ld b, $ff
out (c), a
ld b, $bf
out (c), h ; psg
; initialize interrupts
di ; disables interrupts
ld a, $28 ; a = 40 (specifies address)
ld i, a ; i = a
im 2 ; mode 2 interrupts
ei ; enable interrupts
ret

org 24550
;----------
; bank_set
; inputs (order of and target register): d = bank to switch to
;----------
            ; BEGIN - load parameters via BASIC DEF FN which populates parameters at DEFADD
            ld hl, (DEFADD)
            inc hl
            inc hl
            inc hl
            inc hl
            ld d, (hl) ; d = bank to switch to
            ; END - load parameters
            ld a, (BANK_M) ; read BANK_M
            and %11111000 ; reset first 3 bytes
            or d ; adjust first 3 bytes with the value of "bank"
            ld bc, BANK_P ; port where we want to perform the OUT
            di ; disable interrupts
            ld (BANK_M), a ; update BANK_M
            out (c), a ; perform the OUT
            ei ; enable interrupts
            ld b, 0
            ld c, 0 ; return value is 0
            ret

include "pasmo\clotho.asm"

org 32348
include "pasmo\int.asm"