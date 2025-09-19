SECTION BANK_6

; -------------------------------------------------------------------
; AY Chip frequencies to be loaded in register 0 (lhs - fine tuning) then register 1 (rhs - course tuning)
; -------------------------------------------------------------------
c0:    equ $0D3D
cs0:   equ $0C7F
d0:    equ $0BCC
ds0:   equ $0B22
e0:    equ $0A82
f0:    equ $09EB
fs0:   equ $095D
g0:    equ $08D6
gs0:   equ $0857
a0:    equ $07DF
as0:   equ $076E
b0:    equ $0703
c1:    equ $069F
cs1:   equ $0640
d1:    equ $05E6
ds1:   equ $0591
e1:    equ $0541
f1:    equ $04F6
fs1:   equ $04AE
g1:    equ $046B
gs1:   equ $042C
a1:    equ $03F0
as1:   equ $03B7
b1:    equ $0382
c2:    equ $034F
cs2:   equ $0320
d2:    equ $02F3
ds2:   equ $02C8
e2:    equ $02A1
f2:    equ $027B
fs2:   equ $0257
g2:    equ $0236
gs2:   equ $0216
a2:    equ $01F8
as2:   equ $01DC
b2:    equ $01C1
c3:    equ $01A8
cs3:   equ $0190
d3:    equ $0179
ds3:   equ $0164
e3:    equ $0150
f3:    equ $013D
fs3:   equ $012C
g3:    equ $011B
gs3:   equ $010B
a3:    equ $00FC
as3:   equ $00EE
b3:    equ $00E0
c4:    equ $00D4
cs4:   equ $00C8
d4:    equ $00BD
ds4:   equ $00B2
e4:    equ $00A8
f4:    equ $009F
fs4:   equ $0096
g4:    equ $008D
gs4:   equ $0085
a4:    equ $007E
as4:   equ $0077
b4:    equ $0070
c5:    equ $006A
cs5:   equ $0064
d5:    equ $005E
ds5:   equ $0059
e5:    equ $0054
f5:    equ $004F
fs5:   equ $004B
g5:    equ $0047
gs5:   equ $0043
a5:    equ $003F
as5:   equ $003B
b5:    equ $0038
c6:    equ $0035
cs6:   equ $0032
d6:    equ $002F
ds6:   equ $002D
e6:    equ $002A
f6:    equ $0028
fs6:   equ $0025
g6:    equ $0023
gs6:   equ $0021
a6:    equ $001F
as6:   equ $001E
b6:    equ $001C
c7:    equ $001A
cs7:   equ $0019
d7:    equ $0018
ds7:   equ $0016
e7:    equ $0015
f7:    equ $0014
fs7:   equ $0013
g7:    equ $0012
gs7:   equ $0011
a7:    equ $0010
as7:   equ $000F
b7:    equ $000E
c8:    equ $000D
cs8:   equ $000C
d8:    equ $000C
ds8:   equ $000B
e8:    equ $000B
f8:    equ $000A
fs8:   equ $0009
g8:    equ $0009
gs8:   equ $0008

PUBLIC _tracker_init
PUBLIC _tracker_play
PUBLIC _tracker
PUBLIC _clotho_channel1_score
PUBLIC _clotho_channel2_score
PUBLIC _clotho_channel3_score
PUBLIC _clotho_score

;-------------
; tracker_init
;-------------
_tracker_init:
            ; initialize ay mixer to all notes but no noises
            ld a, 7
            ld h, %00111000
            ld c, $fd
            ld b, $ff
            out (c), a
            ld b, $bf
            out (c), h ; psg
            ret

;-------------
; tracker_play
;-------------
_tracker_play:
            ; opt - c never changes, weirdly (used in psg calls only)
            ld c, $fd
            ; load tracker
            ld hl, (tracker_note)
            ld a, (hl) ; a = tracker
            or a ; populate flags
            jr nz, tracker_play_continue ; if empty then reset, else jump to continue
            ; set all current notes to beginning of scores
            ld hl, (tracker_channel1_start)
            ld (tracker_channel1_note), hl
            ld hl, (tracker_channel2_start)
            ld (tracker_channel2_note), hl
            ld hl, (tracker_channel3_start)
            ld (tracker_channel3_note), hl
            ld hl, (tracker_start)
            ld (tracker_note), hl
            ld a, (hl) ; a = tracker
tracker_play_continue:
            ; play notes based on tracker in a
            ld de, $0800 ; d = channel 1 volume regiser (8), e = channel 1 fine tune register (0)
            ld hl, tracker_channel1_note ; hl = channel 1 current note
            call tracker_play_note
            ; channel 2
            ld hl, (tracker_note)
            ld a, (hl) ; a = tracker
            rrca
            rrca ; ch2 into position 0
            ld de, $0902 ; d = channel 2 volume regiser (9), e = channel 2 fine tune register (2)
            ld hl, tracker_channel2_note ; hl = channel 2 current note
            call tracker_play_note
            ; channel 3
            ld hl, (tracker_note)
            ld a, (hl) ; a = tracker
            rrca
            rrca
            rrca
            rrca ; ch3 into position 0
            ld de, $0a04 ; d = channel 3 volume regiser (10), e = channel 3 fine tune register (4)
            ld hl, tracker_channel3_note ; hl = channel 3 current note
            call tracker_play_note
            ; overall envelope settings
            ld a,11
            ld h,32 ; 11 and 12 govern duration of envelope
            ld b,$ff
            out (c),a
            ld b,$bf
            out (c),h ; psg
            inc a            
            ld b,$ff
            out (c),a
            ld b,$bf
            out (c),h ; psg
            inc a
            ld h,1 ; use envelope 1 (see page 155)
            ld b,$ff
            out (c),a
            ld b,$bf
            out (c),h ; psg
            ; move tracker to next memory location
            ld hl, tracker_note
            inc (hl)
            ret nz
            inc hl
            inc (hl)
            ret
;-------------
; tracker_play_note
; inputs - a: tracker, d: channel volume register, e: channel fine tune register, hl: channel current note
;-------------
tracker_play_note:
            and %00000011 ; only keep bits 0 and 1 of tracker so can check against 0-3
            jr nz, tracker_play_note_continue_1 ; if result of and is non zero then jump to continue
            ld a, d ; no - silence note
            ld h, 0
            ld b,$ff
            out (c),a
            ld b,$bf
            out (c),h ; psg
            ret
tracker_play_note_continue_1:
            ; a is 1-3, quick calc to get relative volume
            inc a ; 2,3,4
            add a, a ; 4,6,8
            add a, a ; 8,12,16 (16=use envelope)
            ld b,$ff
            out (c), d
            ld b,$bf
            out (c), a ; psg
            cp 16 ; use envelope?
            jr nz, tracker_play_note_continue_2 ; use envelope? no, continue using default volume
            ; yes, also progress note to next
            ld a, (hl)
            add a, 2
            ld (hl), a
            jr nc, tracker_play_note_continue_2
            inc hl
            inc (hl)
            dec hl ; reset pointer to current note
tracker_play_note_continue_2:            ; load values
            ld a, e ; a = fine tune register
            ; retrieve current note...
            ld e, (hl)
            inc hl
            ld d, (hl)
            ex de, hl ; hl = address pointed to by channel1_note
            ; ...into d and e...
            ld d, (hl) ; d = rhs - fine tune (rhs as little endian)
            inc hl
            ld e, (hl) ; e = lhs - course tune (lhs as little endian)
            ; ...and set fine/course tune registers
            ld b,$ff
            out (c), a
            ld b,$bf
            out (c), d ; psg
            inc a ; a = course tune register
            ld b,$ff
            out (c), a
            ld b,$bf
            out (c), e ; psg
            ret

_tracker:
db 10 ; tracker.note_wait
tracker_channel1_note:
dw $0000 ; tracker.channel1_note
tracker_channel2_note:
dw $0000 ; tracker.channel2_note
tracker_channel3_note:
dw $0000 ; tracker.channel3_note
tracker_note:
dw _clotho_score_end ; tracker.note
tracker_channel1_start:
dw _clotho_channel1_score - 2 ; tracker.channel1_start - first new note moves into position
tracker_channel2_start:
dw _clotho_channel2_score - 2 ; tracker.channel2_start - first new note moves into position
tracker_channel3_start:
dw _clotho_channel3_score - 2 ; tracker.channel3_start - first new note moves into position
tracker_start:
dw _clotho_score ; tracker.start

_clotho_channel1_score:
dw a3
dw e3
dw c4
dw e3
dw b3
dw e3
dw a3
dw e3
dw gs3
dw e3
dw b3
dw e3
dw a3
dw e3
dw gs3
dw e3
dw g3
dw d3
dw b3
dw d3
dw a3
dw d3
dw g3
dw d3
dw fs3
dw d3
dw g3
dw d3
dw a3
dw d3
dw g3
dw d3
dw a3
dw e3
dw c4
dw e3
dw b3
dw e3
dw a3
dw e3
dw d4
dw e3
dw b3
dw e3
dw gs3
dw e3
dw b3
dw e3
dw g3
dw d3
dw b3
dw d3
dw a3
dw d3
dw c4
dw d3
dw b3
dw d3
dw g3
dw d3
dw fs3
dw d3
dw g3
dw d3
dw c3
dw c3
dw d3
dw b2
dw g2
dw d2
dw g2
dw a2
dw b2
dw a2
dw g2
dw a2
dw fs2
dw g2
dw a2
dw fs2
dw d2
dw e2
dw fs2
dw d2
dw e3
dw c3
dw d3
dw e3
dw d3
dw e3
dw d3
dw b2
dw gs2
dw b2
dw c3
dw d3
dw b2
dw c3
dw d3
dw e3
dw c3
dw a2
dw b2
dw c3
dw a2
dw d3
dw c3
dw b2
dw c3
dw d3
dw b2
dw c3
dw d3
dw gs2
dw a2
dw b2
dw c3
dw fs2
dw b2
dw g2
dw e2
dw a2
dw g2
dw fs2
dw e2
dw a2
dw fs2
dw d2
dw e2
dw a2
dw fs2
dw d2
dw d3
dw b2
dw c3
dw b2
dw a2
dw b2
dw c3
dw d3
dw gs2
dw a2
dw b2
dw c3
dw fs2
dw g2
dw b2
dw a2
dw g2
dw fs2
dw a2
dw g2
dw fs2
dw e2
dw a2
dw fs2
dw d2
dw e2
dw a2
dw fs2
dw d2
dw e2
dw a2
dw fs2
dw d2
dw e2
dw a2
dw b2
dw c3
dw f2
dw g2
dw f2
dw e2
dw a2
dw b2
dw c3
dw f2
dw d3
dw c3
dw b2
dw e3
dw c3
dw d3
dw b2
dw c3
dw a2
dw gs2
dw a2
dw b2
dw gs2
dw a2
dw b2
dw gs2
dw a2
dw b2
dw gs2
dw a2
dw a2
dw b2
dw c3
dw a2
dw b2
dw c3
dw a2
dw b2
dw b2
dw c3
dw d3
dw b2
dw c3
dw d3
dw b2
dw c3
dw g3
dw a3
dw f3
dw g3
dw e3
dw f3
dw d3
dw e3
dw gs2
dw a2
dw b2
dw gs2
dw a2
dw b2
dw gs2
dw a2
dw a2
dw b2
dw c3
dw a2
dw b2
dw c3
dw a2
dw b2
dw e3
dw c3
dw d3
dw b2
dw gs2
dw a2
dw a2

_clotho_channel2_score:
dw a1
dw e1
dw a1
dw c2
dw b1
dw gs1
dw e1
dw gs1
dw g1
dw b1
dw d2
dw g1
dw b1
dw d1
dw g1
dw b1
dw a1
dw e1
dw a1
dw b1
dw c2
dw b1
dw a1
dw e1
dw b1
dw a1
dw gs1
dw a1
dw gs1
dw e1
dw d1
dw e1
dw d2
dw fs2
dw d2
dw c2
dw b1
dw a1
dw b1
dw d2
dw a1
dw b1
dw c2
dw a1
dw fs1
dw g1
dw a1
dw fs1
dw a1
dw b1
dw e1
dw e1
dw gs1
dw g1
dw e1
dw c2
dw a1
dw fs1
dw d1
dw a1
dw fs1
dw b1
dw g1
dw d1
dw g1
dw b1
dw b1
dw c2
dw d1
dw gs1
dw a1
dw b1
dw c2
dw fs1
dw g1
dw b1
dw g1
dw e1
dw d1
dw d1
dw a1
dw d1
dw a1
dw d1
dw g1
dw a1
dw a1
dw e1
dw gs1
dw b1
dw e1
dw a1
dw fs1
dw d1
dw fs1
dw e1
dw b1
dw g1
dw e1
dw d1
dw a1
dw fs1
dw d1
dw a1
dw d1
dw a1
dw d1
dw a1
dw d1
dw a1
dw a1
dw f1
dw c2
dw as1
dw a1
dw a1
dw f1
dw f1
dw e1
dw d1
dw a2
dw g2
dw f2
dw e2
dw e2
dw e1
dw e2
dw c2
dw f2
dw f2
dw d2
dw g2
dw g2
dw e2
dw d2
dw b1
dw c2
dw a1
dw b1
dw g1
dw a1
dw f1
dw gs2
dw gs2
dw e2
dw a2
dw c3
dw b2
dw gs1
dw a1
dw a1

_clotho_channel3_score:
dw g2
dw a2
dw b2
dw e2
dw a2
dw b2
dw c3
dw a2
dw fs2
dw g2
dw a2
dw d2
dw b2
dw g2
dw g2
dw a2
dw g2
dw a2
dw f2
dw e2
dw d2
dw cs2
dw a2
dw f2
dw a2
dw g2
dw g2
dw c3
dw a2
dw b2
dw g2
dw a2
dw f2
dw e2
dw fs2
dw gs2
dw e1
dw f1
dw g1
dw d1
dw e2
dw c1
dw d3
dw b2
dw c2
dw a2
dw b2
dw gs1
dw a1
dw a2
dw g2
dw e2

_clotho_score:
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10000011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001110
db %10001110
db %10001110
db %10001110
db %10001110
db %10001111
db %10001111
db %10001111
db %10001110
db %10001110
db %10001110
db %10001110
db %10001110
db %10001110
db %10001110
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001010
db %10001011
db %10001011
db %10001111
db %10001010
db %10001011
db %10001010
db %10001111
db %10001010
db %10001111
db %10001010
db %10001111
db %10001010
db %10001011
db %10001010
db %10111111
db %10111011
db %10111111
db %10111011
db %10111111
db %10111011
db %10111111
db %10111011
db %10111111
db %10111111
db %10111111
db %10111111
db %10111111
db %10111111
db %10111111
db %10111111
db %10111111
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001010
db %10001111
db %10001010
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001010
db %10001010
db %10001010
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001011
db %10001111
db %10001010
db %10001010
db %10001010
db %10111111
db %10101011
db %10101011
db %10111111
db %10101010
db %10101010
db %10101010
db %10101010
db %10000000
db %10000000
db %10000000
db %10000000
db %10111111
db %10101010
db %10111111
db %10101010
db %10111111
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10111111
db %10101011
db %10101011
db %10111111
db %10101010
db %10101010
db %10101010
db %10101010
db %10000000
db %10000000
db %10000000
db %10000000
db %10111111
db %10101010
db %10111111
db %10101010
db %10111111
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10111111
db %10111011
db %10111111
db %10111011
db %10111111
db %10111011
db %10111111
db %10111011
db %10111111
db %10101010
db %10101010
db %10101010
db %10101010
db %10101010
db %10111111
db %10101011
db %10101011
db %10101011
db %10101011
db %10101011
db %10001111
db %10001111
db %10111111
db %10101011
db %10101011
db %10101011
db %10101011
db %10101011
db %10001111
db %10001111
db %10111111
db %10101011
db %10101011
db %10101011
db %10101011
db %10101011
db %10001111
db %10001111
db %10111111
db %10111111
db %10111111
db %10111111
db %10111111
db %10111111
db %10111111
db %10111111
db %10111111
db %10101011
db %10101011
db %10101011
db %10101011
db %10101011
db %10001111
db %10001111
db %10111111
db %10101011
db %10101011
db %10101011
db %10101011
db %10101011
db %10101011
db %10101011
db %10111111
db %10101011
db %10111111
db %10101011
db %10111111
db %10101010
db %10101010
db %10001111
db %10001111
db %10001010
db %10001010
db %10001010
db %10001010
db %10001010
db %10001010
db %10001010
_clotho_score_end:
db %00000000 ; end