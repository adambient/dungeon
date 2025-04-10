#include <stdlib.h>
#include "globals.h"
#include "player.h"
#include "int.h"
#include <im2.h>
#include <intrinsic.h>

// TODO - this is obviously hard coded to be [16][2] so perhaps we should generate this
static unsigned char map[MAP_SIZE][MAP_SIZE / 4] = {
    // walls                  // crates 
    { 0b11111111, 0b11111111, 0b00000000, 0b00000000},
    { 0b10000001, 0b10000001, 0b00000000, 0b00000000},
    { 0b10101101, 0b10110101, 0b00000000, 0b00000000},
    { 0b10000000, 0b00000001, 0b00100000, 0b00000100},
    { 0b10101101, 0b11010101, 0b00000000, 0b00000000},
    { 0b10001101, 0b10010001, 0b00100000, 0b00000100},
    { 0b10100001, 0b10110101, 0b00000000, 0b00000000},
    { 0b10111111, 0b10000101, 0b00000000, 0b00000000},
    { 0b10100001, 0b11111101, 0b00000000, 0b00000000},
    { 0b10101101, 0b10000101, 0b00000000, 0b00000000},
    { 0b10001001, 0b10110001, 0b00000000, 0b00000000},
    { 0b10101011, 0b10110101, 0b00000000, 0b00000000},
    { 0b10000000, 0b00000001, 0b00100000, 0b00000100},
    { 0b10101101, 0b10110101, 0b00000000, 0b00000000},
    { 0b10000001, 0b10000001, 0b00100000, 0b00000100},
    { 0b11111111, 0b11111111, 0b00000000, 0b00000000},
};

// imported from map.asm
extern unsigned char get_map_tile(unsigned char x, unsigned char y) __z88dk_callee;
extern void set_map_tile(unsigned char x, unsigned char y, unsigned int tile)  __z88dk_callee;
// imported from fill_rectangle.asm
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;
extern void bright_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width) __z88dk_callee;
extern void copy_attr_buffer(void) __z88dk_callee; // copy attribute buffer into attribute memory

unsigned char *start_attr_address;
unsigned char *attr_address;
unsigned char map_frame;

static inline unsigned char row_get_tile(unsigned char x, unsigned char y)
{
    if (x < MAP_SIZE && y < MAP_SIZE)
    {
        unsigned char tile = get_map_tile(x, y);
        if (tile & 0b00000001 == 0b00000001) // has tile been seen
        {
            return (tile << 2 & 0b00111000);
        }
    }

    return 0;
}

void row_draw_vertical(signed char x, signed char x2, unsigned char y)
{
    unsigned char i = VISIBLE_BLOCKS;
    for (; i > 0; i--)
    {
        unsigned char tile = row_get_tile(x, y);
        unsigned char tile2 = x == x2 ? tile : row_get_tile(x2, y);
        tile = tile >> 3 | tile2;
        *attr_address++ = tile;
        *attr_address++ = tile;
        y++;
    }
    for (i = 15; i >= VISIBLE_BLOCKS; i--)
    {
        attr_address++;
        attr_address++;
    }
}

void row_draw_horizontal(signed char x, unsigned char y)
{    
    for (unsigned char i = VISIBLE_BLOCKS; i < 255; i--)
    {
        unsigned char tile = row_get_tile(x, y);
        unsigned char tile2 = tile;
        switch (map_frame)
        {
            case 1:
            case 3:
                tile = row_get_tile(x, y - 1);
                break;
        }

        if (i == 0)
        {
            switch (map_frame)
            {
                case 1:
                case 2:
                    // catch up last block (first skipped)
                    *attr_address++ = tile >> 3 | tile;
                    break;
            }
            attr_address++;
            attr_address++;
        }
        else
        {
            if (map_frame == 0 || map_frame == 3 || i < VISIBLE_BLOCKS)
            {
                // do not skip first block
                *attr_address++ = tile >> 3 | tile;
            }
            *attr_address++ = tile >> 3 | tile2;
            y++;
        }
    }
    for (unsigned char i = 15; i > VISIBLE_BLOCKS; i--)
    {        
        attr_address++;
        attr_address++;
    }
}

void map_draw_vertical(void)
{    
    attr_address = start_attr_address; // reset shared attr_address    
    unsigned char sub_frame = 0;
    switch (map_frame)
    {
        case 1:
        case 3:
            sub_frame++;
            break;
    }
    signed char x = player_x - MAP_OFFSET - 1; // starting row (could be negative)
    unsigned char y = player_y - MAP_OFFSET;
    switch (map_frame)
    {
        case 2:
        case 3:
            row_draw_vertical(x, x, y);
            break;
    }
    signed char rows = player_x + MAP_OFFSET;
    for (x = x + 1; x < rows; x++)
    {
        row_draw_vertical(x - sub_frame, x, y);
        row_draw_vertical(x, x, y);
    }
    row_draw_vertical(x - sub_frame, x, y);
    if (map_frame < 2)
    {
        // catch up row
        row_draw_vertical(x, x, y);
    }    
}

void map_draw_horizontal(void)
{
    attr_address = start_attr_address; // reset shared attr_address    
    unsigned char y = player_y - MAP_OFFSET;
    if (map_frame == 2)
    {
        y--;
    }
    for (signed char x = player_x - MAP_OFFSET; x <= player_x + MAP_OFFSET; x++)
    {
        row_draw_horizontal(x, y);
        row_draw_horizontal(x, y);
    }    
}

void map_init(void)
{
    start_attr_address = (unsigned char*)(ATTR_BUFF); // start of map attribute memory    
    for (unsigned char x = MAP_SIZE - 1; x < 255; x--) {
        for (unsigned char y = (MAP_SIZE / 8) - 1; y < 255; y--) {
            unsigned char i = 0;
            unsigned char m = map[x][y]; // map
            unsigned char c = map[x][y + 2]; // crate
            for (unsigned char i = 7; i < 255; i--) {
                unsigned char tile = BLUE; // wall
                if ((c << i & 0b10000000) == 0b10000000) {
                    // use crate data
                    tile = YELLOW; // crate
                } else {
                    // use map data                    
                    if ((m << i & 0b10000000) == 0b00000000) {
                        if (((x + i) & 0b00000001) == 0b00000001) {
                            tile = RED; // carpet 1
                        } else {
                            tile = MAGENTA; // carpet 2
                        }
                    }
                }
                set_map_tile(x, i + (8 * y), tile << 1);
            }
        }
    }
}

void map_move_up(void)
{   
    unsigned char tile = get_map_tile(player_x - 1, player_y);
    if ((tile & 0b00001110) == 0b00000010)
    {
        // tile is inaccessible
        return;
    }

    if ((tile & 0b00001110) == 0b00001100)
    {
        // tile is a crate
        unsigned char next_tile = get_map_tile(player_x - 2, player_y);
        if ((next_tile & 0b00001110) == 0b00000010 || (next_tile & 0b00001110) == 0b00001100)
        {
            // next tile is blocked
            return;
        }

        // replace crate with carpet pattern
        set_map_tile(player_x - 1, player_y, (RED  | (player_x + player_y & 0b00000001)) << 1 | 0b00000001);
        is_player_pushing = 1;            
    }

    player_draw_up(); 
    // animate up            
    map_frame = 1;    
    map_draw_vertical();
    player_draw_background_vertical();
    intrinsic_halt();
    copy_attr_buffer();
    map_frame++;
    map_draw_vertical();
    player_draw_background_vertical();
    intrinsic_halt();
    copy_attr_buffer();
    map_frame++;
    map_draw_vertical();
    player_draw_background_vertical();
    intrinsic_halt();
    copy_attr_buffer();
    player_x--;    
    map_frame = 0;
    player_see(3, 2, 2, 2); // final position    
    map_draw_vertical();
    player_draw_background_vertical();
    if (is_player_pushing == 1) {
        // place crate
        is_player_pushing = 0;
        set_map_tile(player_x - 1, player_y, YELLOW << 1 | 0b00000001);
    }
    intrinsic_halt();
    copy_attr_buffer();
}

void map_move_down(void)
{   
    unsigned char tile = get_map_tile(player_x + 1, player_y);
    if ((tile & 0b00001110) == 0b00000010)
    {
        // tile is inaccessible
        return;
    }

    if ((tile & 0b00001110) == 0b00001100)
    {
        // tile is a crate
        unsigned char next_tile = get_map_tile(player_x + 2, player_y);
        if ((next_tile & 0b00001110) == 0b00000010 || (next_tile & 0b00001110) == 0b00001100)
        {
            // next tile is blocked
            return;
        }

        // replace crate with carpet pattern
        set_map_tile(player_x + 1, player_y, (RED  | (player_x + player_y & 0b00000001)) << 1 | 0b00000001);
        is_player_pushing = 1;
    }
    
    player_draw_down();
    // animate down                 
    map_frame = 3;    
    player_x++;
    map_draw_vertical();
    player_draw_background_vertical();
    intrinsic_halt();
    copy_attr_buffer();
    map_frame--;
    map_draw_vertical();
    player_draw_background_vertical();
    intrinsic_halt();
    copy_attr_buffer();
    map_frame--;
    map_draw_vertical(); 
    player_draw_background_vertical();
    intrinsic_halt();
    copy_attr_buffer();
    map_frame--;
    player_see(2, 3, 2, 2); // final position    
    map_draw_vertical();
    player_draw_background_vertical();
    if (is_player_pushing == 1) {
        // place crate
        is_player_pushing = 0;
        set_map_tile(player_x + 1, player_y, YELLOW << 1 | 0b00000001);
    }
    intrinsic_halt();
    copy_attr_buffer();
}

void map_move_left(void)
{       
    unsigned char tile = get_map_tile(player_x, player_y - 1);
    if ((tile & 0b00001110) == 0b00000010)
    {
        // tile is inaccessible
        return;
    }

    if ((tile & 0b00001110) == 0b00001100)
    {
        // tile is a crate
        unsigned char next_tile = get_map_tile(player_x, player_y - 2);
        if ((next_tile & 0b00001110) == 0b00000010 || (next_tile & 0b00001110) == 0b00001100)
        {
            // next tile is blocked
            return;
        }

        // replace crate with carpet pattern
        set_map_tile(player_x, player_y - 1, (RED  | (player_x + player_y & 0b00000001)) << 1 | 0b00000001);
        is_player_pushing = 1;
    }

    player_draw_left();
    // animate left
    map_frame = 1;        
    map_draw_horizontal();
    player_draw_background_horizontal();
    intrinsic_halt();
    copy_attr_buffer();
    map_frame++;
    map_draw_horizontal();
    player_draw_background_horizontal();
    intrinsic_halt();
    copy_attr_buffer();
    map_frame++;
    map_draw_horizontal();
    player_draw_background_horizontal();
    intrinsic_halt();
    copy_attr_buffer();
    player_y--;
    map_frame = 0;
    player_see(2, 2, 3, 2); // final position    
    map_draw_horizontal();
    player_draw_background_horizontal();
    if (is_player_pushing == 1) {
        // place crate
        is_player_pushing = 0;
        set_map_tile(player_x, player_y - 1, YELLOW << 1 | 0b00000001);
    }
    intrinsic_halt();
    copy_attr_buffer();
}

void map_move_right(void)
{
    unsigned char tile = get_map_tile(player_x, player_y + 1);
    if ((tile & 0b00001110) == 0b00000010)
    {
        // tile is inaccessible
        return;
    }

    if ((tile & 0b00001110) == 0b00001100)
    {
        // tile is a crate
        unsigned char next_tile = get_map_tile(player_x, player_y + 2);
        if ((next_tile & 0b00001110) == 0b00000010 || (next_tile & 0b00001110) == 0b00001100)
        {
            // next tile is blocked
            return;
        }

        // replace crate with carpet pattern
        set_map_tile(player_x, player_y + 1, (RED  | (player_x + player_y & 0b00000001)) << 1 | 0b00000001);
        is_player_pushing = 1;
    }

    player_draw_right();
    // animate right     
    map_frame = 3;        
    player_y++;
    map_draw_horizontal();
    player_draw_background_horizontal();
    intrinsic_halt();
    copy_attr_buffer();
    map_frame--;
    map_draw_horizontal();
    player_draw_background_horizontal();
    intrinsic_halt();
    copy_attr_buffer();
    map_frame--;
    map_draw_horizontal();
    player_draw_background_horizontal();
    intrinsic_halt();
    copy_attr_buffer();
    map_frame--;
    player_see(2, 2, 2, 3); // final position
    map_draw_horizontal();
    player_draw_background_horizontal();
    if (is_player_pushing == 1) {
        // place crate
        is_player_pushing = 0;
        set_map_tile(player_x, player_y + 1, YELLOW << 1 | 0b00000001);
    }
    intrinsic_halt();
    copy_attr_buffer();
}