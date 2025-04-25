#include <im2.h>
#include <intrinsic.h>
#include <string.h>
#include <z80.h>
#include "int.h"

extern void copy_attr_buffer(void) __z88dk_callee; // copy attribute buffer into attribute memory

// timer
unsigned char int_tick;
unsigned char int_outer_tick;
unsigned char int_colour;
unsigned char int_timer;

IM2_DEFINE_ISR(isr)
{
    // update the clock
    ++int_tick;
}

void int_refresh_screen(void)
{
    ++int_colour;    
    if (int_colour > 7)
    {
        int_colour = 0;
        ++int_outer_tick;
    }
    intrinsic_halt();
    copy_attr_buffer();
}

void int_init(void)
{
    im2_init((void *)0xd000); // CRT_ORG = 25124
    memset((void *)0xd000, 0xd1, 257);

    z80_bpoke(0xd1d1, 0xc3);
    z80_wpoke(0xd1d2, (unsigned int)isr);

    intrinsic_ei();
}