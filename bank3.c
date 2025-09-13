#include <intrinsic.h>
#include <z80.h>
#include "globals.h"
#include "screen.h"
#include <stdio.h>
#include "fill_rectangle.h"
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
    fill_rectangle_attr_inline(0, 0, 24, 32, 7, 7);
    intrinsic_halt();
    copy_attr_buffer();
    fill_rectangle_char_inline(0, 0, 24, 32, " $"); // clear screen
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
    fill_rectangle_attr_inline(0, 0, 24, 32, BLACK, WHITE);
    // make the word green, green
    fill_rectangle_attr_inline(5, 24, 1, 3, GREEN, WHITE);
    // make the word gold, yellow
    fill_rectangle_attr_inline(7, 27, 1, 2, YELLOW, WHITE);
    fill_rectangle_char_inline(0, 0, VISIBLE_BLOCKS * 2, VISIBLE_BLOCKS * 2, "[$"); // fill with pipes    
}

void bank3_screen_success(void)
{    
    fill_rectangle_char_inline(6, 6, 5, 20, " $");
    puts("\x16\x0E\x08 Congratulations you have escaped the");
    puts("\x16\x0E\x09 dark and mysterious labyrinth! Press");
    puts("\x16\x0E\x0A Enter to try again...");
    fill_rectangle_attr_inline(6, 6, 5, 20, BLACK, WHITE);    
    bright_rectangle_attr_inline(6, 6, 5, 20);
    intrinsic_halt();
    copy_attr_buffer();    
}

void bank3_map_init(void)
{
    enemy_init();
    map_uncovered_holes = 0;
    for (unsigned char x = MAP_SIZE - 1; x < 255; x--)
    {
        for (unsigned char y = (MAP_SIZE / 8) - 1; y < 255; y--)
        {
            unsigned char i = 0;
            for (unsigned char i = 7; i < 255; i--)
            {
                unsigned char tile = WALL; // wall
                if ((map[x][y + 2] << i & 0b10000000) == 0b10000000)
                {
                    // use crate data
                    tile = CRATE;
                }
                else
                {
                    // use map data
                    if ((map[x][y] << i & 0b10000000) == 0b00000000)
                    {
                        if (((x + i) & 0b00000001) == 0b00000001)
                        {
                            tile = CARPET_1; // carpet 1
                        }
                        else
                        {
                            tile = CARPET_2; // carpet 2
                        }
                    }
                    // could also be a target
                    if ((map[x][y + 4] << i & 0b10000000) == 0b10000000)
                    {
                        tile = TARGET;
                        map_uncovered_holes++;
                    }
                    // colour also be an enemy
                    if ((map[x][y + 6] << i & 0b10000000) == 0b10000000)
                    {
                        enemy_add(x, i + (8 * y));
                    }
                }
                set_grid_tile_inline(x, i + (8 * y), tile);
            }
        }
    }
}