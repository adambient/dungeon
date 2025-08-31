/*
NOTE - We are using newlib which defines sections for our ASM, not using these as appropriate seems to cause strange issues
SECTION code_user: assign executable code to this section
SECTION rodata_user: assign read-only data to this section
SECTION smc_user: assign self-modifying code to this section
SECTION data_user: assign non-zero initial data to this section
SECTION bss_user: assign zero initial data to this section
*/

#include "globals.h"
#include "screen.h"
#include "map.h"
#include <input.h>
#include "beeps.h"
#include "enemy.h"
#include "int.h"

void init(void)
{        
    map_init();        
    player_x = MAP_SIZE - 1;
    player_y = 1;    
    player_dir = DIR_UP; // move up first of all to draw map TODO - shouldn't need to do this    
    map_move_none();
}

void main(void)
{    
    screen_init();
    int_init();
    init();
    do
    {
        // player move/draw
        switch (player_dir)
        {
        case DIR_UP:
            player_dir = map_move_up();
            break;
        case DIR_RIGHT:
            player_dir = map_move_right();
            break;
        case DIR_DOWN:
            player_dir = map_move_down();
            break;
        case DIR_LEFT:
            player_dir = map_move_left();
            break;
        }

        if (player_dir == DIR_NONE)
        {
            map_move_none();
        }

        // check for player movement
        player_dir = DIR_NONE;
        if (in_key_pressed(IN_KEY_SCANCODE_q) == 0xFFFF)
        {
            player_dir = DIR_UP;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_p) == 0xFFFF)
        {
            player_dir = DIR_RIGHT;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_a) == 0xFFFF)
        {
            player_dir = DIR_DOWN;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_o) == 0xFFFF)
        {
            player_dir = DIR_LEFT;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_SPACE) ==0xFFFF)
        {
            is_player_pushing = 1;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_g) ==0xFFFF)
        {
            display_attrs = 0;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_h) ==0xFFFF)
        {
            display_attrs = 1;
        }

        if (ENEMY == player_tile)
        {            
            init();
        }

        enemy_move();
        play_sounds();

        // end game check
        if (map_uncovered_holes == 0)
        {
            if ((player_x == 1 && (player_y == (MAP_SIZE - 2) || player_y == 1)) ||
                (player_x == (MAP_SIZE - 2) && (player_y == (MAP_SIZE - 2) || player_y == 1)))
            {
                screen_success();
                do
                {
                } while (in_key_pressed(IN_KEY_SCANCODE_ENTER) !=0xFFFF);

                player_facing = DIR_NONE;
                screen_init();
                init();
            }
        }

    } while (1);
}