#include "globals.h"

// imported from map.asm
extern unsigned char get_map_tile(unsigned char x, unsigned char y) __z88dk_callee;

unsigned char enemy_x = 3;
unsigned char enemy_y = 3;
static unsigned char enemy_dir = DIR_UP;
static unsigned char enemy_move_cycle = 0;
#define ENEMY_MOVE_CYCLE 5

static unsigned char enemy_attempt_move(unsigned char next_dir)
{
    if (next_dir > DIR_LEFT)
    {
        next_dir = next_dir - DIR_NONE;
    }
    unsigned char next_x = enemy_x;
    unsigned char next_y = enemy_y;
    switch (next_dir)
    {
    case DIR_UP:
        next_x--;
        break;
    case DIR_RIGHT:
        next_y++;
        break;
    case DIR_DOWN:
        next_x++;
        break;
    case DIR_LEFT:
        next_y--;
        break;
    }
    unsigned char next_tile = (get_map_tile(next_x, next_y) & BG_BYTES);
    if (next_tile == CARPET_1 || next_tile == CARPET_2 || next_tile == TARGET)
    {
        enemy_x = next_x;
        enemy_y = next_y;
        enemy_dir = next_dir;
        return 1;
    }
    else
    {
        return 0;
    }
}

void enemy_move(void)
{
    if (enemy_move_cycle < ENEMY_MOVE_CYCLE)
    {
        enemy_move_cycle++;
    }
    else
    {
        enemy_move_cycle = 0;
        // TODO - we are just cycling around up, right, down, left until we find a direction we can go it. Should be more intelligent
        for (unsigned char dir = enemy_dir; dir < (enemy_dir + DIR_NONE); dir++)
        {
            if (enemy_attempt_move(dir))
            {
                break;
            }
        }
    }
}