#include <intrinsic.h>
#include <z80.h>
#include "globals.h"
#include "screen.h"
#include <stdio.h>

// imported from screen.asm
extern void print_string(uint8_t *string) __z88dk_fastcall; // print null terminated string, accepts rst $10 control codes

// imported from fill_rectangle.asm
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;
extern void copy_attr_buffer(void) __z88dk_callee; // copy attribute buffer into attribute memory

// timer
unsigned char screen_colour_cycle;
unsigned char screen_colour;

void screen_init(void)
{    
    intrinsic_ei(); // enable interrupts
    print_string("Initialising...");
    fill_rectangle_char(0, 0, 24, 32, " "); // repeating background pattern
    fill_rectangle_attr(0, 0, 24, 32, 7, 7);
    fill_rectangle_char(0, 0, VISIBLE_BLOCKS * 2, VISIBLE_BLOCKS * 2, "["); // fill with pipes
    puts("\x16\x01\x17Q:up;A:down;L:left;R:right;Sp:action:G:gfx off;H:gfx on");
}

void screen_refresh(void)
{
    ++screen_colour;    
    if (screen_colour > 7)
    {
        screen_colour = 0;
        ++screen_colour_cycle;
    }
    intrinsic_halt();
    copy_attr_buffer();
}