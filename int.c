#include <im2.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>
#include "int.h"

extern void copy_attr_buffer(void) __z88dk_callee; // copy attribute buffer into attribute memory

// timer
unsigned char tick;
unsigned char timer;

IM2_DEFINE_ISR(isr)
{    
    // update the clock
    ++tick;
}

void refresh_screen(void) {
    intrinsic_halt();
    copy_attr_buffer();
}

void wait(void)
{
    while (abs(tick - timer) < WFRAMES) {
        refresh_screen();
    }

    timer = tick;
}

void setup_int(void)
{
    im2_init((void *)0xd000); // CRT_ORG = 25124
    memset((void *)0xd000, 0xd1, 257);

    z80_bpoke(0xd1d1, 0xc3);
    z80_wpoke(0xd1d2, (unsigned int)isr);

    intrinsic_ei();
}