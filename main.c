/*
NOTE - We are using newlib which defines sections for our ASM, not using these as appropriate seems to cause strange issues
SECTION code_user: assign executable code to this section
SECTION rodata_user: assign read-only data to this section
SECTION smc_user: assign self-modifying code to this section
SECTION data_user: assign non-zero initial data to this section
SECTION bss_user: assign zero initial data to this section
*/

#include <stdlib.h>
#include "globals.h"
#include "map.h"
#include <input.h>
#include "int.h"
#include "beeps.h"
#include "enemy.h"

// imported from screen.asm
extern void print_string(uint8_t *string) __z88dk_fastcall; // print null terminated string, accepts rst $10 control codes

// imported from fill_rectangle.asm
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;

void main(void)
{
    int_init();
    print_string("Initialising...");
    fill_rectangle_char(0, 0, 24, 32, " "); // repeating background pattern
    fill_rectangle_attr(0, 0, 24, 32, 7, 7);
    int_refresh_screen();
    map_init();
    fill_rectangle_char(0, 0, VISIBLE_BLOCKS * 2, VISIBLE_BLOCKS * 2, "["); // fill with pipes
    player_x = MAP_SIZE - 1;
    player_y = 1;
    player_dir = DIR_UP; // move up first of all to draw map TODO - shouldn't need to do this
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

        // resolve enemy movement
        enemy_move();

        play_sounds();
        int_wait();
    } while (1);
}