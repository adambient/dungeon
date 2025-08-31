#include <intrinsic.h>
#include <z80.h>
#include "globals.h"
#include "screen.h"
#include <stdio.h>

// imported from fill_rectangle.asm
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;
extern void bright_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width) __z88dk_callee;
extern void copy_attr_buffer(void) __z88dk_callee; // copy attribute buffer into attribute memory

// timer
unsigned char screen_colour_cycle;
unsigned char screen_colour;

void screen_init(void)
{    
    intrinsic_ei(); // enable interrupts
    fill_rectangle_attr(0, 0, 24, 32, 7, 7);
    intrinsic_halt();
    copy_attr_buffer();
    fill_rectangle_char(0, 0, 24, 32, " $"); // clear screen
    puts("\x16\x26\x01 ZX DUNGEON");
    puts("\x16\x26\x02 ==========");
    puts("\x16\x26\x04 You wake up in a dark and "); // max string length
    puts("\x16\x26\x05 mysterious labarynth.");
    puts("\x16\x26\x06 Beware of green enemies.");
    puts("\x16\x26\x07 Block holes in the fabric");
    puts("\x16\x26\x08 of reality with gold then");
    puts("\x16\x26\x09 find the secret exit."); 
    puts("\x16\x26\x0C Harelquins: INFINITE");
    puts("\x16\x26\x0D Enemies: 4");
    puts("\x16\x26\x0E Carried: Turnip");
    puts("\x16\x26\x10 Q:up;");
    puts("\x16\x26\x11 A:down;");
    puts("\x16\x26\x12 L:left;");
    puts("\x16\x26\x13 R:right;");
    puts("\x16\x26\x14 Sp:action;");
    puts("\x16\x26\x15 G:gfx off;");
    puts("\x16\x26\x16 H:gfx on;");        
    puts("\x16\x01\x14 A dark and mysterious labyrinth.");
        
    // default text is black on white
    fill_rectangle_attr(0, 0, 24, 32, BLACK, WHITE);
    // make the word green, green
    fill_rectangle_attr(5, 24, 1, 3, GREEN, WHITE);
    // make the word gold, yellow
    fill_rectangle_attr(7, 27, 1, 2, YELLOW, WHITE);
    fill_rectangle_char(0, 0, VISIBLE_BLOCKS * 2, VISIBLE_BLOCKS * 2, "[$"); // fill with pipes    
}

void screen_refresh(void)
{    
    // cycle colour for the text "holes"
    fill_rectangle_attr(6, 22, 1, 3, screen_colour, WHITE);
    ++screen_colour;    
    if (screen_colour > WHITE)
    {
        screen_colour = BLACK;
        ++screen_colour_cycle;
    }
    intrinsic_halt();
    copy_attr_buffer();
}

void screen_success(void)
{    
    fill_rectangle_char(6, 6, 5, 20, " $");
    puts("\x16\x0E\x08 Congratulations you have escaped the");
    puts("\x16\x0E\x09 dark and mysterious labyrinth! Press");
    puts("\x16\x0E\x0A Enter to try again...");
    fill_rectangle_attr(6, 6, 5, 20, BLACK, WHITE);    
    bright_rectangle_attr(6, 6, 5, 20);
    intrinsic_halt();
    copy_attr_buffer();    
}