#include <im2.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

// imported from tracker.asm
extern void tracker_init(void) __z88dk_callee;
extern void tracker_play(void) __z88dk_callee;

unsigned char tick;

IM2_DEFINE_ISR(isr)
{    
    // update the clock
    ++tick;
    if (tick == 10)
    {
        tracker_play();
        tick = 0;
    }
}

void int_init(void)
{
    tracker_init();

    im2_init((void *)0xd000); // CRT_ORG = 25124
    memset((void *)0xd000, 0xd1, 257);

    z80_bpoke(0xd1d1, 0xc3);
    z80_wpoke(0xd1d2, (unsigned int)isr);

    intrinsic_ei();
}