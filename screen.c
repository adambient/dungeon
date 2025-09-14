#include <intrinsic.h>
#include <z80.h>
#include "globals.h"
#include "screen.h"
#include <stdio.h>
#include "gfx.h"

// timer
unsigned char screen_colour_cycle;
unsigned char screen_colour;

void screen_refresh(void)
{    
    // cycle colour for the text "holes"
    gfx.x = 6;
    gfx.y = 22;
    gfx.height = 1;
    gfx.width = 3;
    gfx.ink = screen_colour;
    gfx.paper = WHITE;
    gfx_attr(&gfx);
    ++screen_colour;    
    if (screen_colour > WHITE)
    {
        screen_colour = BLACK;
        ++screen_colour_cycle;
    }
    intrinsic_halt();
    gfx_flush();
}