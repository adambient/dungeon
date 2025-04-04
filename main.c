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

// imported from screen.asm
extern void print_string(uint8_t *string) __z88dk_fastcall; // print null terminated string, accepts rst $10 control codes

// imported from fill_rectangle.asm
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;
extern void copy_attr_buffer(void) __z88dk_callee; // copy attribute buffer into attribute memory

static inline unsigned char move_up(void)
{
    if (player_x > 0)
    {
        map_move_up();
        return 1;
    }

    return 0;
}

static inline unsigned char move_right(void)
{
    if (player_y < MAP_SIZE - 1)
    {
        map_move_right();
        return 1;
    }

    return 0;
}

static inline unsigned char move_down(void)
{
    if (player_x < MAP_SIZE - 1)
    {
        map_move_down();
        return 1;
    }

    return 0;
}

static inline unsigned char move_left(void)
{
    if (player_y > 0)
    {
        map_move_left();
        return 1;
    }

    return 0;
}

void main(void)
{       
    print_string("Initialising...");    
    map_init();    
    fill_rectangle_char(0, 0, 24, 32, " "); // repeating background pattern
    fill_rectangle_attr(0, 0, 24, 32, 0, 7);
    copy_attr_buffer();
    player_x = MAP_SIZE - 1;
    player_y = 1;
    player_dir = 0; // move up first of all to draw map TODO - shouldn't need to do this
    do
    {
        // loop around map
        switch (player_dir)
        {
            default:
                // stand still TODO - torch should still flicker
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
    } while (1);
}