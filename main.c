#pragma output CRT_ORG_CODE = 0x8000 // just after basic loader and pasmo bank switcher
#pragma output REGISTER_SP = 0xBFE0 // end of uncontended memory, so not switched out by banking

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
#include "bank3.h"
#include "banker.h"
#include <stdio.h>
#include "int.h"

extern void pager_set(unsigned char bank) __z88dk_fastcall;

void init(void)
{
    screen_init(); 
    exec_far(bank3_map_init, 3);
    globals.player_x = MAP_SIZE - 1;
    globals.player_y = 1;
    globals.player_facing = DIR_NONE;
    globals.player_dir = DIR_UP; // move up first of all to draw map TODO - shouldn't need to do this    
    map_move_none();
}

void main(void)
{
    int_init();
    init();
    do
    {
        // player move/draw
        switch (globals.player_dir)
        {
        case DIR_UP:
            globals.player_dir = map_move_up();
            break;
        case DIR_RIGHT:
            globals.player_dir = map_move_right();
            break;
        case DIR_DOWN:
            globals.player_dir = map_move_down();
            break;
        case DIR_LEFT:
            globals.player_dir = map_move_left();
            break;
        }

        if (globals.player_dir == DIR_NONE)
        {
            map_move_none();
        }

        // check for player movement
        globals.player_dir = DIR_NONE;
        if (in_key_pressed(IN_KEY_SCANCODE_q) == 0xFFFF)
        {
            globals.player_dir = DIR_UP;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_p) == 0xFFFF)
        {
            globals.player_dir = DIR_RIGHT;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_a) == 0xFFFF)
        {
            globals.player_dir = DIR_DOWN;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_o) == 0xFFFF)
        {
            globals.player_dir = DIR_LEFT;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_SPACE) ==0xFFFF)
        {
            globals.is_player_pushing = 1;
        }

        if (ENEMY == globals.player_tile)
        {            
            exec_far(beeps_death, 4);
            init();
        }

        enemy_move();
        exec_far(beeps_play, 4);

        // end game check
        if (map_uncovered_holes == 0)
        {
            if ((globals.player_x == 1 && (globals.player_y == (MAP_SIZE - 2) || globals.player_y == 1)) ||
                (globals.player_x == (MAP_SIZE - 2) && (globals.player_y == (MAP_SIZE - 2) || globals.player_y == 1)))
            {
                screen_success();
                exec_far(beeps_winner, 4);
                do
                {
                } while (in_key_pressed(IN_KEY_SCANCODE_ENTER) !=0xFFFF);

                globals.player_facing = DIR_NONE;
                init();
            }
        }

    } while (1);
}