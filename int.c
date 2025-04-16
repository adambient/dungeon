#include <im2.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>
#include "int.h"

extern void copy_attr_buffer(void) __z88dk_callee; // copy attribute buffer into attribute memory

// timer
unsigned char tick;
unsigned char colour;
unsigned char timer;

IM2_DEFINE_ISR(isr)
{
    // update the clock
    ++tick;
}

void int_refresh_screen(void)
{
    ++colour;
    if (colour > 7)
    {
        colour = 0;
    }
    intrinsic_halt();
    copy_attr_buffer();
}

void int_wait(void)
{
    while (abs(tick - timer) < WFRAMES)
    {
        intrinsic_halt();
    }

    timer = tick;
}

void int_init(void)
{
    im2_init((void *)0xd000); // CRT_ORG = 25124
    memset((void *)0xd000, 0xd1, 257);

    z80_bpoke(0xd1d1, 0xc3);
    z80_wpoke(0xd1d2, (unsigned int)isr);

    intrinsic_ei();
}