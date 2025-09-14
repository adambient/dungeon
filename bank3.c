#include <intrinsic.h>
#include <z80.h>
#include "globals.h"
#include "screen.h"
#include <stdio.h>
#include "gfx.h"
#include "enemy.h"
#include "map.h"
#include "grid.h"

// TODO - this is obviously hard coded to be [16][8] so perhaps we should generate this, also is it optimal space wise?
static unsigned char map[MAP_SIZE][(MAP_SIZE / 8) * 4] = {
    // walls                 // BLOCKs               // target squares       // enemies
    {0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10000001, 0b10000001, 0b00000000, 0b00000000, 0b00000100, 0b00100000, 0b00000000, 0b00000000},
    {0b10101101, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10000000, 0b00000001, 0b00100000, 0b00000100, 0b00000100, 0b00100000, 0b00010000, 0b00010000},
    {0b10101101, 0b11010101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10001101, 0b10010001, 0b00100000, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10100001, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10111111, 0b10000101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10100001, 0b11111101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10101101, 0b10000101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10001001, 0b10110001, 0b00100000, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10101011, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10000000, 0b00000001, 0b00100000, 0b00000100, 0b00000100, 0b00100000, 0b00010000, 0b00010000},
    {0b10101101, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10000001, 0b10000001, 0b00000000, 0b00000000, 0b00000100, 0b00100000, 0b00000000, 0b00000000},
    {0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
};

void bank3_function(unsigned int value)
{
    printf("We're in bank 3, at %p\n",bank3_function);
    printf("And we were sent a value of %d\n",value);    
    printf("Back we go to the main program!\n");
}

void bank3_screen_init(void)
{    
    intrinsic_ei();
    // hide screen
    gfx.x = 0;
    gfx.y = 0;
    gfx.height = 24;
    gfx.width = 32;
    gfx.ink = WHITE;
    gfx.paper = WHITE;
    gfx_attr(&gfx);
    intrinsic_halt();
    gfx_flush();
    // clear screen
    gfx.height = 24;
    gfx.width = 32;
    gfx.c = " $";
    gfx_char(&gfx); 
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
    puts("\x16\x26\x15 G:gfx off;");
    puts("\x16\x26\x16 H:gfx on;");        
    puts("\x16\x01\x14 A dark and mysterious labyrinth.");
        
    // default text is black on white
    gfx.x = 0;
    gfx.y = 0;
    gfx.height = 24;
    gfx.width = 32;
    gfx.ink = BLACK;
    gfx.paper = WHITE;
    gfx_attr(&gfx);
    // fill the playable area with pipes
    gfx.height = VISIBLE_BLOCKS * 2;
    gfx.width = VISIBLE_BLOCKS * 2;
    gfx.c = "[$";
    gfx_char(&gfx); 
    // make the word green, green
    gfx.x = 5;
    gfx.y = 24;
    gfx.height = 1;
    gfx.width = 3;
    gfx.ink = GREEN;
    gfx_attr(&gfx);
    // make the word gold, yellow
    gfx.x = 7;
    gfx.y = 27;
    gfx.width = 2;
    gfx.ink = YELLOW;
    gfx_attr(&gfx);
}

void bank3_screen_success(void)
{    
    gfx.x = 6;
    gfx.y = 6;
    gfx.height = 5;
    gfx.width = 20;
    gfx.c = " $";
    gfx_char(&gfx);
    puts("\x16\x0E\x08 Congratulations you have escaped the");
    puts("\x16\x0E\x09 dark and mysterious labyrinth! Press");
    puts("\x16\x0E\x0A Enter to try again...");
    gfx.ink = BLACK;
    gfx.paper = WHITE;
    gfx_attr(&gfx);
    gfx_bright(&gfx);
    intrinsic_halt();
    gfx_flush();    
}

void bank3_map_init(void)
{
    enemy_init();
    map_uncovered_holes = 0;
    for (grid.x = MAP_SIZE - 1; grid.x < 255; grid.x--)
    {
        for (unsigned char y = (MAP_SIZE / 8) - 1; y < 255; y--)
        {
            unsigned char i = 0;
            for (unsigned char i = 7; i < 255; i--)
            {
                grid.tile = WALL; // wall
                if ((map[grid.x][y + 2] << i & 0b10000000) == 0b10000000)
                {
                    // use crate data
                    grid.tile = CRATE;
                }
                else
                {
                    // use map data
                    if ((map[grid.x][y] << i & 0b10000000) == 0b00000000)
                    {
                        if (((grid.x + i) & 0b00000001) == 0b00000001)
                        {
                            grid.tile = CARPET_1; // carpet 1
                        }
                        else
                        {
                            grid.tile = CARPET_2; // carpet 2
                        }
                    }
                    // could also be a target
                    if ((map[grid.x][y + 4] << i & 0b10000000) == 0b10000000)
                    {
                        grid.tile = TARGET;
                        map_uncovered_holes++;
                    }
                    // colour also be an enemy
                    if ((map[grid.x][y + 6] << i & 0b10000000) == 0b10000000)
                    {
                        enemy_add(grid.x, i + (8 * y));
                    }
                }
                grid.y = i + (8 * y);
                grid_set(&grid);
            }
        }
    }
}