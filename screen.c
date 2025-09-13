#include <intrinsic.h>
#include <z80.h>
#include "globals.h"
#include "screen.h"
#include <stdio.h>
#include "fill_rectangle.h"

// timer
unsigned char screen_colour_cycle;
unsigned char screen_colour;

void screen_refresh(void)
{    
    // cycle colour for the text "holes"
    fill_rectangle_attr_inline(6, 22, 1, 3, screen_colour, WHITE);
    ++screen_colour;    
    if (screen_colour > WHITE)
    {
        screen_colour = BLACK;
        ++screen_colour_cycle;
    }
    intrinsic_halt();
    copy_attr_buffer();
}