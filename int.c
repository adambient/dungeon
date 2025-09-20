#include <im2.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>
#include "tracker.h"
#include "banker.h"

unsigned char tick;

void my_isr_body(void) {
    ++tick;
    unsigned char current_bank = banker_get();
    banker_set(4);
    if (tick == tracker.note_wait)
    {
        tracker_play();
        tick = 0;
    }
    banker_set(current_bank);
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
    unsigned char current_bank = banker_get();
    banker_set(4);
    tracker_init();
    banker_set(current_bank);
    im2_init((void *)0xbd00);
    memset((void *)0xbd00, 0xbe, 257);
    z80_bpoke(0xbebe, 0xc3);
    z80_wpoke(0xbebf, (unsigned int)my_isr);
    intrinsic_ei();
}