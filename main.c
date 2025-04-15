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

// imported from screen.asm
extern void print_string(uint8_t *string) __z88dk_fastcall; // print null terminated string, accepts rst $10 control codes

// imported from fill_rectangle.asm
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;

unsigned char last_player_dir;

static void inline move_none(void)
{
    player_dir = last_player_dir;
    map_move_none();
    player_dir = 4;
}

static void inline move_up(void)
{
    last_player_dir = player_dir;
    if (player_x > 0)
    {
        map_move_up();
        return;
    }
    move_none();
}

static void inline move_right(void)
{
    last_player_dir = player_dir;
    if (player_y < MAP_SIZE - 1)
    {
        map_move_right();
        return;
    }
    move_none();
}

static void inline move_down(void)
{
    last_player_dir = player_dir;
    if (player_x < MAP_SIZE - 1)
    {
        map_move_down();
        return;
    }
    move_none();
}

static void inline move_left(void)
{
    last_player_dir = player_dir;
    if (player_y > 0)
    {
        map_move_left();
        return;
    }
    move_none();
}

void main(void)
{       
    setup_int();
    print_string("Initialising...");    
    fill_rectangle_char(0, 0, 24, 32, " "); // repeating background pattern    
    fill_rectangle_attr(0, 0, 24, 32, 7, 7);
    refresh_screen();
    map_init();
    fill_rectangle_char(0, 0, VISIBLE_BLOCKS * 2, VISIBLE_BLOCKS * 2, "["); // fill with pipes
    player_x = MAP_SIZE - 1;
    player_y = 1;
    player_dir = 0; // move up first of all to draw map TODO - shouldn't need to do this
    do
    {        
        // loop around map
        switch (player_dir)
        {
            default:
                move_none();
                break;
            case 0:
                move_up();
                break;
            case 1:
                move_right();
                break;
            case 2:
                move_down();
                break;
            case 3:
                move_left();
                break;
        }

        // check for movement                
        player_dir = 4;
        if (in_key_pressed(IN_KEY_SCANCODE_q) == 0xFFFF) {
            player_dir = 0;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_p) == 0xFFFF) {
            player_dir = 1;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_a) == 0xFFFF) {
            player_dir = 2;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_o) == 0xFFFF) {
            player_dir = 3;
        }
        
        wait();
    } while (1);
}