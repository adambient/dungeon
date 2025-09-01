#include <im2.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

// imported from tracker.asm
extern void tracker_init(void) __z88dk_callee;
extern void tracker_play(void) __z88dk_callee;

unsigned char tick;

void my_isr_body(void) {    
    ++tick;
    if (tick == 10)
    {
        tracker_play();
        tick = 0;
    }
}

// opt - we know that we only update these registers so can avoid some push/pops by using __interrupt __naked
void my_isr(void) __interrupt __naked {
    __asm
        push af
        push bc
        push de
        push hl

        call _my_isr_body     ; call the C body

        pop  hl
        pop  de
        pop  bc
        pop  af
        ei
        reti
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