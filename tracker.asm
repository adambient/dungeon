; -------------------------------------------------------------------
; AY Chip frequencies to be loaded in register 0 (lhs - fine tuning) then register 1 (rhs - course tuning)
; NOTE - remember Z80 is little endian so when reading bytes the rhs is read first
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

SECTION code_user

PUBLIC _tracker_init

;-----------------
; tracker_init
;-----------------
_tracker_init:
            ld a, 7
            ld h, %00111000
            ld c, $fd
            ld b, $ff
            out (c), a
            ld b, $bf
            out (c), h ; psg
            ret

SECTION data_user

PUBLIC _tracker_tick_count
PUBLIC _tracker_note_wait
PUBLIC _tracker_channel1_note
PUBLIC _tracker_channel2_note
PUBLIC _tracker_channel3_note
PUBLIC _tracker_note
PUBLIC _tracker_channel1_start
PUBLIC _tracker_channel2_start
PUBLIC _tracker_channel3_start
PUBLIC _tracker_start
PUBLIC _tracker_channel1_score
PUBLIC _tracker_channel2_score
PUBLIC _tracker_channel3_score
PUBLIC _tracker_score
PUBLIC _tracker_score_end

_tracker_tick_count:  db 1 ; count between notes
_tracker_note_wait:
db 10 ; wait 0.2 seconds between notes (PAL)

_tracker_channel1_note:
dw $0000
_tracker_channel2_note:
dw $0000
_tracker_channel3_note:
dw $0000
_tracker_note:
dw _tracker_score_end

_tracker_channel1_start:
dw _tracker_channel1_score - 2 ; first new note moves into position
_tracker_channel2_start:
dw _tracker_channel2_score - 2 ; first new note moves into position
_tracker_channel3_start:
dw _tracker_channel3_score - 2 ; first new note moves into position
_tracker_start:
dw _tracker_score

_tracker_channel1_score:
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

_tracker_channel2_score:
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

_tracker_channel3_score:
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

_tracker_score:
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
_tracker_score_end:
db %00000000 ; end