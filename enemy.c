#include "globals.h"
#include "screen.h"
#include "enemy.h"
#include "grid.h"

#define MAX_ENEMIES 4

struct enemy_data
{
    unsigned char x;
    unsigned char y;
    unsigned char dir;
};

static struct enemy_data enemies[MAX_ENEMIES];

typedef struct {
    unsigned char count;
    unsigned char tick;
} enemy_t;

static enemy_t enemy;




void enemy_init(void)
{
    enemy.count = 0;
    enemy.tick = 0;
}

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

    if (enemy_is_located(next_x, next_y))
    {
        // enemy alrady located on tile
        return 0;
    }

    grid.x = next_x;
    grid.y = next_y;
    grid_get();
    unsigned char next_tile = (grid.tile & BG_BYTES);
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
    if (enemy.count < MAX_ENEMIES)
    {
        enemies[enemy.count].x = x;
        enemies[enemy.count].y = y;
        enemies[enemy.count].dir = DIR_UP;
        enemy.count++;
    }
}

#define ENEMY_FOLLOW_N 5
void enemy_move(void)
{
    // move every outer tick change
    if (enemy.tick != screen_colour_cycle)
    {
        enemy.tick = screen_colour_cycle;
        for (unsigned char enemy_index = 0; enemy_index < enemy.count; enemy_index++)
        {
            // enemies follow the player if within ENEMY_FOLLOW_N squares (TODO which is still too hard)
            signed char diff = globals.player_x - enemies[enemy_index].x;
            if (diff > 0 && diff < ENEMY_FOLLOW_N)
            {
                if (enemy_attempt_move(DIR_DOWN, enemy_index))
                {
                    continue;
                }
            }
            else if (diff < 0 && diff > -ENEMY_FOLLOW_N)
            {
                if (enemy_attempt_move(DIR_UP, enemy_index))
                {
                    continue;
                }
            }
            diff = globals.player_y - enemies[enemy_index].y;
            if (diff > 0 && diff < ENEMY_FOLLOW_N)
            {
                if (enemy_attempt_move(DIR_RIGHT, enemy_index))
                {
                    continue;
                }
            }
            else if (diff < 0 && diff > -ENEMY_FOLLOW_N)
            {
                if (enemy_attempt_move(DIR_LEFT, enemy_index))
                {
                    continue;
                }
            }
        }
    }
}

unsigned char enemy_is_located(unsigned char x, unsigned char y)
{
    for (unsigned char enemy_index = 0; enemy_index < enemy.count; enemy_index++)
    {
        if (x == enemies[enemy_index].x && y == enemies[enemy_index].y)
        {
            return 1;
        }
    }

    return 0;
}