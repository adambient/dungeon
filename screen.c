#include <intrinsic.h>
#include <z80.h>
#include "globals.h"
#include "screen.h"
#include <stdio.h>
#include "gfx.h"

screen_t screen;

void screen_init(void)
{
    // hide screen
    gfx.x = 0;
    gfx.y = 0;
    gfx.height = 24;
    gfx.width = 32;
    gfx.ink = WHITE;
    gfx.paper = WHITE;
    gfx_attr();
    intrinsic_halt();
    gfx_flush();
    // clear screen
    gfx.height = 24;
    gfx.width = 32;
    gfx.c = " $";
    gfx.bank = &gfx_bank0;
    gfx_char(); 
    puts("\x16\x26\x01 The D.A.M. Labyrinth");
    puts("\x16\x26\x02 ====================");
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
    puts("\x16\x26\x12 O:left;");
    puts("\x16\x26\x13 P:right;");
    puts("\x16\x26\x14 Sp:action;");     
    puts("\x16\x01\x14 A dark and mysterious labyrinth.");
        
    // default text is black on white
    gfx.x = 0;
    gfx.y = 0;
    gfx.height = 24;
    gfx.width = 32;
    gfx.ink = BLACK;
    gfx.paper = WHITE;
    gfx_attr();
    // fill the playable area with pipes
    gfx.height = VISIBLE_BLOCKS * 2;
    gfx.width = VISIBLE_BLOCKS * 2;
    gfx.c = "[$";
    gfx.bank = &gfx_bank0;
    gfx_char(); 
    // make the word green, green
    gfx.x = 5;
    gfx.y = 24;
    gfx.height = 1;
    gfx.width = 3;
    gfx.ink = GREEN;
    gfx_attr();
    // make the word gold, yellow
    gfx.x = 7;
    gfx.y = 27;
    gfx.width = 2;
    gfx.ink = YELLOW;
    gfx_attr();
}

void screen_success(void)
{    
    gfx.x = 6;
    gfx.y = 6;
    gfx.height = 5;
    gfx.width = 20;
    gfx.c = " $";
    gfx.bank = &gfx_bank0;
    gfx_char();
    puts("\x16\x0E\x08 Congratulations you have escaped the");
    puts("\x16\x0E\x09 dark and mysterious labyrinth! Press");
    puts("\x16\x0E\x0A Enter to try again...");
    gfx.ink = BLACK;
    gfx.paper = WHITE;
    gfx_attr();
    gfx_bright();
    intrinsic_halt();
    gfx_flush();    
}

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