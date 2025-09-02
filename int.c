#include <im2.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

// imported from tracker.asm
extern unsigned char tracker_tick_count;
extern unsigned char tracker_note_wait;
extern unsigned char *tracker_channel1_note;
extern unsigned char *tracker_channel2_note;
extern unsigned char *tracker_channel3_note;
extern unsigned char *tracker_note;
extern unsigned char *tracker_channel1_start;
extern unsigned char *tracker_channel2_start;
extern unsigned char *tracker_channel3_start;
extern unsigned char *tracker_start;
extern unsigned char *tracker_channel1_score;
extern unsigned char *tracker_channel2_score;
extern unsigned char *tracker_channel3_score;
extern unsigned char *tracker_score;
extern unsigned char *tracker_score_end;
extern void tracker_init(void) __z88dk_callee;

// opt - our entire tracker can fit in the interrupt routine
void my_isr(void) __interrupt __naked {
    __asm
            ; save all registers
            push af
            push bc
            push de
            push hl
            ; wait for tracker_note_wait ticks
            ld hl, _tracker_tick_count ; load count
            ld a, (hl) ; into a
            dec (hl) ; decrease count
            jr nz, my_isr_end ; end if not zero
            ld a, (_tracker_note_wait)
            ld (hl), a ; reset count
            call tracker_play ; call tracker to play next notes
            ; END - interrupt routine
        my_isr_end:
            ; retrieve all saved registers
            pop  hl
            pop  de
            pop  bc
            pop  af
            ei ; activates interruptions
            reti ; exits
        ;-----------------
        ; tracker_play
        ;-----------------
        tracker_play:
                    ; opt - c never changes, weirdly (used in psg calls only)
                    ld c, 0xfd
                    ; load tracker
                    ld hl, (_tracker_note)
                    ld a, (hl) ; a = tracker
                    or a ; populate flags
                    jr nz, tracker_play_continue ; if empty then reset, else jump to continue
                    ; set all current notes to beginning of scores
                    ld hl, (_tracker_channel1_start)
                    ld (_tracker_channel1_note), hl
                    ld hl, (_tracker_channel2_start)
                    ld (_tracker_channel2_note), hl
                    ld hl, (_tracker_channel3_start)
                    ld (_tracker_channel3_note), hl
                    ld hl, (_tracker_start)
                    ld (_tracker_note), hl
                    ld a, (hl) ; a = tracker
        tracker_play_continue:
                    ; play notes based on tracker in a
                    ld de, 0x0800 ; d = channel 1 volume regiser (8), e = channel 1 fine tune register (0)
                    ld hl, _tracker_channel1_note ; hl = channel 1 current note
                    call tracker_play_note
                    ; channel 2
                    ld hl, (_tracker_note)
                    ld a, (hl) ; a = tracker
                    rrca
                    rrca ; ch2 into position 0
                    ld de, 0x0902 ; d = channel 2 volume regiser (9), e = channel 2 fine tune register (2)
                    ld hl, _tracker_channel2_note ; hl = channel 2 current note
                    call tracker_play_note
                    ; channel 3
                    ld hl, (_tracker_note)
                    ld a, (hl) ; a = tracker
                    rrca
                    rrca
                    rrca
                    rrca ; ch3 into position 0
                    ld de, 0x0a04 ; d = channel 3 volume regiser (10), e = channel 3 fine tune register (4)
                    ld hl, _tracker_channel3_note ; hl = channel 3 current note
                    call tracker_play_note
                    ; overall envelope settings
                    ld a,11
                    ld h,32 ; 11 and 12 govern duration of envelope
                    ld b,0xff
                    out (c),a
                    ld b,0xbf
                    out (c),h ; psg
                    inc a            
                    ld b,0xff
                    out (c),a
                    ld b,0xbf
                    out (c),h ; psg
                    inc a
                    ld h,1 ; use envelope 1 (see page 155)
                    ld b,0xff
                    out (c),a
                    ld b,0xbf
                    out (c),h ; psg
                    ; move tracker to next memory location
                    ld hl, _tracker_note
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
                    ld b,0xff
                    out (c),a
                    ld b,0xbf
                    out (c),h ; psg
                    ret
        tracker_play_note_continue_1:
                    ; a is 1-3, quick calc to get relative volume
                    inc a ; 2,3,4
                    add a, a ; 4,6,8
                    add a, a ; 8,12,16 (16=use envelope)
                    ld b,0xff
                    out (c), d
                    ld b,0xbf
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
                    ld b,0xff
                    out (c), a
                    ld b,0xbf
                    out (c), d ; psg
                    inc a ; a = course tune register
                    ld b,0xff
                    out (c), a
                    ld b,0xbf
                    out (c), e ; psg
                    ret
    __endasm;
}

void int_init(void)
{
    tracker_init();
    im2_init((void *)0xd000); // CRT_ORG = 25124
    memset((void *)0xd000, 0xd1, 257);
    z80_bpoke(0xd1d1, 0xc3);
    z80_wpoke(0xd1d2, (unsigned int)my_isr);
    intrinsic_ei();
}