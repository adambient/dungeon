SECTION BANK_4

PUBLIC _tracker_init
PUBLIC _tracker_play
PUBLIC _tracker_stop
PUBLIC _tracker

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

_tracker_stop:
            ; initialize ay mixer to no notes and no noises
            ld a, 7
            ld h, %00111111
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
dw $0000 ; tracker.note
tracker_channel1_start:
dw $0000 ; tracker.channel1_start
tracker_channel2_start:
dw $0000 ; tracker.channel2_start
tracker_channel3_start:
dw $0000 ; tracker.channel3_start
tracker_start:
dw $0000 ; tracker.start
_tracker_init_score:
db %00000000 ; point tracker note here to reset current score (any 0 byte would work)