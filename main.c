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

static void inline move_none(void)
{
    player_dir = player_facing; // so the correct kind of refresh happens
    map_move_none();
    player_dir = 4;
}

static void inline move_up(void)
{
    if (player_x > 0)
    {
        map_move_up();
        return;
    }
    move_none();
}

static void inline move_right(void)
{
    if (player_y < MAP_SIZE - 1)
    {
        map_move_right();
        return;
    }
    move_none();
}

static void inline move_down(void)
{
    if (player_x < MAP_SIZE - 1)
    {
        map_move_down();
        return;
    }
    move_none();
}

static void inline move_left(void)
{
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
    player_dir = DIR_UP; // move up first of all to draw map TODO - shouldn't need to do this
    do
    {        
        // loop around map
        switch (player_dir)
        {
            default:
                move_none();
                break;
            case DIR_UP:
                move_up();
                break;
            case DIR_RIGHT:
                move_right();
                break;
            case DIR_DOWN:
                move_down();
                break;
            case DIR_LEFT:
                move_left();
                break;
        }

        // check for movement                
        player_dir = DIR_NONE;
        if (in_key_pressed(IN_KEY_SCANCODE_q) == 0xFFFF) {
            player_dir = DIR_UP;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_p) == 0xFFFF) {
            player_dir = DIR_RIGHT;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_a) == 0xFFFF) {
            player_dir = DIR_DOWN;
        }
        if (in_key_pressed(IN_KEY_SCANCODE_o) == 0xFFFF) {
            player_dir = DIR_LEFT;
        }
        
        wait();
    } while (1);
}