#include <intrinsic.h>
#include <z80.h>
#include "globals.h"
#include "screen.h"
#include <stdio.h>
#include "gfx.h"

screen_t screen;

void screen_refresh(void)
{    
    // cycle colour for the text "holes"
    gfx.x = 6;
    gfx.y = 22;
    gfx.height = 1;
    gfx.width = 3;
    gfx.ink = screen.colour;
    gfx.paper = WHITE;
    gfx_attr();
    ++screen.colour;    
    if (screen.colour > WHITE)
    {
        screen.colour = BLACK;
        ++screen.colour_cycle;
    }
    intrinsic_halt();
    gfx_flush();
}