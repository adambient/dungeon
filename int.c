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
extern void tracker_play(void) __z88dk_callee;

void my_isr(void) __naked {
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
            call _tracker_play ; call tracker to play next notes
            ; END - interrupt routine
        my_isr_end:
            ; retrieve all saved registers
            pop  hl
            pop  de
            pop  bc
            pop  af
            ei ; activates interruptions
            reti ; exits
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