#include <intrinsic.h>
#include <z80.h>
#include "globals.h"
#include "screen.h"
#include <stdio.h>

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
    fill_rectangle_attr(0, 0, 24, 32, 7, 7);
    fill_rectangle_char(0, 0, 24, 32, " "); // repeating background pattern
    puts("\x16\x2C\x01 ZX DUNGEON");
    puts("\x16\x2C\x02 ==========");
    puts("\x16\x2C\x04 You wake up in a");
    puts("\x16\x2C\x05 dark and mysterious");
    puts("\x16\x2C\x06 labarynth. Beware of"); // max string length
    puts("\x16\x2C\x07 green enemies. Block");
    puts("\x16\x2C\x08 holes in the fabric");
    puts("\x16\x2C\x09 of reality with gold"); 
    puts("\x16\x2C\x0A then find the exit.");
    puts("\x16\x2C\x0C Harelquins: 3");
    puts("\x16\x2C\x0D Enemies: 1");
    puts("\x16\x2C\x0E Carried: Turnip");
    puts("\x16\x2C\x10 Q:up;");
    puts("\x16\x2C\x11 A:down;");
    puts("\x16\x2C\x12 L:left;");
    puts("\x16\x2C\x13 R:right;");
    puts("\x16\x2C\x14 Sp:action;");
    puts("\x16\x2C\x15 G:gfx off;");
    puts("\x16\x2C\x16 H:gfx on;");        
    puts("\x16\x01\x17 A dark and mysterious labarynth.");
        
    // default text is black on white
    fill_rectangle_attr(0, 0, 24, 32, 0, 7);
    // make the word green, green
    fill_rectangle_attr(6, 22, 1, 3, GREEN, 7);
    // make the word gold, yellow
    fill_rectangle_attr(8, 30, 1, 2, YELLOW, 7);
    fill_rectangle_char(0, 0, VISIBLE_BLOCKS * 2, VISIBLE_BLOCKS * 2, "["); // fill with pipes    
}

void screen_refresh(void)
{
    
    // cycle colour for the text "holes"
    fill_rectangle_attr(7, 22, 1, 3, screen_colour, 7);
    ++screen_colour;    
    if (screen_colour > 7)
    {
        screen_colour = 0;
        ++screen_colour_cycle;
    }
    intrinsic_halt();
    copy_attr_buffer();
}