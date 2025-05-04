#include "globals.h"
#include "screen.h"

#define MAX_ENEMIES 4

// imported from map.asm
extern unsigned char get_map_tile(unsigned char x, unsigned char y) __z88dk_callee;

struct enemy_data
{
    unsigned char x;
    unsigned char y;
    unsigned char dir;
};
static struct enemy_data enemies[MAX_ENEMIES];

static unsigned char enemy_count = 0;
static unsigned char enemy_tick = 0;

static unsigned char enemy_attempt_move(unsigned char next_dir, unsigned char enemy_index)
{
    if (next_dir > DIR_LEFT)
    {
        next_dir = next_dir - DIR_NONE;
    }
    unsigned char next_x = enemies[enemy_index].x;
    unsigned char next_y = enemies[enemy_index].y;
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
        enemies[enemy_index].x = next_x;
        enemies[enemy_index].y = next_y;
        enemies[enemy_index].dir = next_dir;
        return 1;
    }
    else
    {
        return 0;
    }
}

void enemy_add(unsigned char x, unsigned char y)
{
    if (enemy_count < MAX_ENEMIES)
    {
        enemies[enemy_count].x = x;
        enemies[enemy_count].y = y;
        enemies[enemy_count].dir = DIR_UP;
        enemy_count++;
    }
}

void enemy_move(void)
{
    // move every outer tick change
    if (enemy_tick != screen_colour_cycle)
    {
        enemy_tick = screen_colour_cycle;
        for (unsigned char enemy_index = 0; enemy_index < enemy_count; enemy_index++)
        {
            // TODO - we are just cycling around up, right, down, left until we find a direction we can go it. Should be more intelligent
            for (unsigned char dir = enemies[enemy_index].dir; dir < (enemies[enemy_index].dir + DIR_NONE); dir++)
            {
                if (enemy_attempt_move(dir, enemy_index))
                {
                    break;
                }
            }
        }
    }
}

unsigned char enemy_is_located(unsigned char x, unsigned char y)
{
    for (unsigned char enemy_index = 0; enemy_index < enemy_count; enemy_index++)
    {
        if (x == enemies[enemy_index].x && y == enemies[enemy_index].y)
        {
            return 1;
        }
    }

    return 0;
}