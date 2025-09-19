DEFADD EQU 23563 ; location of parameters passed by BASIC
BANK_M EQU $5b5c ; which bank is currently being used
BANK_P EQU $7ffd ; port for paging banks

org 32512
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