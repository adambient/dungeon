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

include "pasmo\clotho.asm"

org 32348
include "pasmo\int.asm"