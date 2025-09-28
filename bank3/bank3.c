#include <intrinsic.h>
#include <z80.h>
#include "globals.h"
#include "screen.h"
#include <stdio.h>
#include "gfx/gfx.h"
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
                grid_set();
            }
        }
    }
}