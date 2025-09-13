BANK_M EQU $5b5c ; which bank is currently being used
BANK_P EQU $7ffd ; port for paging banks

SECTION code_user

PUBLIC _pager_get
PUBLIC _pager_set

;----------
; _pager_get
; returns which bank is currently paged into slot 3
;----------
_pager_get:
            ld h, 0
            ld a, (BANK_M) ; read BANK_M
            and %00000111 ; onlt load first 3 bytes
            ld l, a ; bc = current bank
            ret

;----------
; _pager_set
; inputs: l = bank to page to (fastcall)
;----------
_pager_set:
            ld a, (BANK_M) ; read BANK_M
            and %11111000 ; reset first 3 bytes
            or l ; adjust first 3 bytes with the value of "bank"
            ld bc, BANK_P ; port where we want to perform the OUT
            di ; disable interrupts
            ld (BANK_M), a ; update BANK_M
            out (c), a ; perform the OUT
            ei ; enable interrupts
            ret