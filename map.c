#include <stdlib.h>
#include "globals.h"
#include "player.h"
#include "int.h"

// TODO - this is obviously hard coded to be [16][6] so perhaps we should generate this, also is it optimal space wise?
static unsigned char map[MAP_SIZE][(MAP_SIZE / 8) * 3] = {
    // walls                  // BLOCKs               // target squares
    { 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    { 0b10000001, 0b10000001, 0b00000000, 0b00000000, 0b00000100, 0b00100000},
    { 0b10101101, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    { 0b10000000, 0b00000001, 0b00100000, 0b00000100, 0b00000100, 0b00100000},
    { 0b10101101, 0b11010101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    { 0b10001101, 0b10010001, 0b00100000, 0b00000100, 0b00000000, 0b00000000},
    { 0b10100001, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    { 0b10111111, 0b10000101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    { 0b10100001, 0b11111101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    { 0b10101101, 0b10000101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    { 0b10001001, 0b10110001, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    { 0b10101011, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    { 0b10000000, 0b00000001, 0b00100000, 0b00000100, 0b00000100, 0b00100000},
    { 0b10101101, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    { 0b10000001, 0b10000001, 0b00100000, 0b00000100, 0b00000100, 0b00100000},
    { 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
};

// imported from map.asm
extern unsigned char get_map_tile(unsigned char x, unsigned char y) __z88dk_callee;
extern void set_map_tile(unsigned char x, unsigned char y, unsigned int tile)  __z88dk_callee;
// imported from fill_rectangle.asm
extern unsigned char* attr_buffer;
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;
extern void bright_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width) __z88dk_callee;

unsigned char *start_attr_address;
unsigned char *attr_address;
unsigned char map_frame;

static inline unsigned char row_get_tile(unsigned char x, unsigned char y)
{
    if (x < MAP_SIZE && y < MAP_SIZE)
    {
        unsigned char tile = get_map_tile(x, y);
        if (tile & TG_BYTE) {
            return colour;
        }

        return (tile & BG_BYTES);
    }

    return DARKNESS;
}

void row_draw_vertical(signed char x, signed char x2, unsigned char y)
{
    unsigned char i = VISIBLE_BLOCKS;
    for (; i > 0; i--)
    {
        unsigned char tile = row_get_tile(x, y);
        unsigned char tile2 = x == x2 ? tile : row_get_tile(x2, y);
        tile = tile | tile2 << 3;
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
                    *attr_address++ = tile | tile << 3;
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
                *attr_address++ = tile | tile << 3;
            }
            *attr_address++ = tile | tile2 << 3;
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
    start_attr_address = (unsigned char *)(&attr_buffer); // start of map attribute memory    
    for (unsigned char x = MAP_SIZE - 1; x < 255; x--) {
        for (unsigned char y = (MAP_SIZE / 8) - 1; y < 255; y--) {
            unsigned char i = 0;
            unsigned char m = map[x][y]; // map
            unsigned char c = map[x][y + 2]; // BLOCK
            unsigned char t = map[x][y + 4]; // target
            for (unsigned char i = 7; i < 255; i--) {
                unsigned char tile = WALL; // wall
                if ((c << i & 0b10000000) == 0b10000000) {
                    // use BLOCK data
                    tile = BLOCK; // BLOCK
                } else {
                    // use map data                    
                    if ((m << i & 0b10000000) == 0b00000000) {
                        if (((x + i) & 0b00000001) == 0b00000001) {
                            tile = CARPET_1; // carpet 1
                        } else {
                            tile = CARPET_2; // carpet 2
                        }
                    }
                    if ((t << i & 0b10000000) == 0b10000000) {
                        tile = tile | TG_BYTE;
                    }
                }
                set_map_tile(x, i + (8 * y), tile);
            }
        }
    }
}

void map_move_up(void)
{   
    unsigned char tile = get_map_tile(player_x - 1, player_y);
    if ((tile & BG_BYTES) == WALL)
    {
        // tile is inaccessible
        return;
    }

    if ((tile & BG_BYTES) == BLOCK || (tile & BG_BYTES) == PLACED)
    {
        // tile is a BLOCK or placed BLOCK
        unsigned char next_tile = get_map_tile(player_x - 2, player_y);
        if ((next_tile & BG_BYTES) == WALL || (next_tile & BG_BYTES) == BLOCK || (next_tile & BG_BYTES) == PLACED)
        {
            // next tile is blocked
            return;
        }
        // replace current tile with carpet
        next_tile = (CARPET_1  | (player_x + player_y & 0b00000001));
        if ((tile & BG_BYTES) == PLACED)
        {
            // was placed - replace current tile with target
            next_tile = next_tile | TG_BYTE;
        }
        set_map_tile(player_x - 1, player_y, next_tile);
        is_player_pushing = 1;           
    }

    player_draw_up(); 
    // animate up            
    map_frame = 1;    
    map_draw_vertical();
    player_draw_background_vertical();
    refresh_screen();
    map_frame++;
    map_draw_vertical();
    player_draw_background_vertical();
    refresh_screen();
    map_frame++;
    map_draw_vertical();
    player_draw_background_vertical();
    refresh_screen();
    player_x--;    
    map_frame = 0;
    player_see(); // final position    
    map_draw_vertical();
    player_draw_background_vertical();
    if (is_player_pushing == 1) {
        // place BLOCK
        is_player_pushing = 0;
        unsigned char next_tile = get_map_tile(player_x - 1, player_y);
        if ((next_tile & TG_BYTE) == TG_BYTE) {
            // target location
            set_map_tile(player_x - 1, player_y, PLACED);
            // remove BLOCK as part of player
            map_draw_vertical();
            player_draw_background_vertical();
        } else {
            set_map_tile(player_x - 1, player_y, BLOCK);
        }        
    }
    refresh_screen();
}

void map_move_down(void)
{   
    unsigned char tile = get_map_tile(player_x + 1, player_y);
    if ((tile & BG_BYTES) == WALL)
    {
        // tile is inaccessible
        return;
    }

    if ((tile & BG_BYTES) == BLOCK || (tile & BG_BYTES) == PLACED)
    {
        // tile is a BLOCK or placed BLOCK
        unsigned char next_tile = get_map_tile(player_x + 2, player_y);
        if ((next_tile & BG_BYTES) == WALL || (next_tile & BG_BYTES) == BLOCK || (next_tile & BG_BYTES) == PLACED)
        {
            // next tile is blocked
            return;
        }
        // replace current tile with carpet
        next_tile = (CARPET_1  | (player_x + player_y & 0b00000001));
        if ((tile & BG_BYTES) == PLACED)
        {
            // was placed - replace current tile with target
            next_tile = next_tile | TG_BYTE;
        }
        set_map_tile(player_x + 1, player_y, next_tile);
        is_player_pushing = 1;          
    }
    
    player_draw_down();
    // animate down                 
    map_frame = 3;    
    player_x++;
    map_draw_vertical();
    player_draw_background_vertical();
    refresh_screen();
    map_frame--;
    map_draw_vertical();
    player_draw_background_vertical();
    refresh_screen();
    map_frame--;
    map_draw_vertical(); 
    player_draw_background_vertical();
    refresh_screen();
    map_frame--;
    player_see(); // final position    
    map_draw_vertical();
    player_draw_background_vertical();
    if (is_player_pushing == 1) {
        // place BLOCK
        is_player_pushing = 0;
        unsigned char next_tile = get_map_tile(player_x + 1, player_y);
        if ((next_tile & TG_BYTE) == TG_BYTE) {
            // target location
            set_map_tile(player_x + 1, player_y, PLACED);
            // remove BLOCK as part of player
            map_draw_vertical();
            player_draw_background_vertical();
        } else {
            set_map_tile(player_x + 1, player_y, BLOCK);
        } 
    }
    refresh_screen();
}

void map_move_left(void)
{       
    unsigned char tile = get_map_tile(player_x, player_y - 1);
    if ((tile & BG_BYTES) == WALL)
    {
        // tile is inaccessible
        return;
    }

    if ((tile & BG_BYTES) == BLOCK || (tile & BG_BYTES) == PLACED)
    {
        // tile is a BLOCK or placed BLOCK
        unsigned char next_tile = get_map_tile(player_x, player_y - 2);
        if ((next_tile & BG_BYTES) == WALL || (next_tile & BG_BYTES) == BLOCK || (next_tile & BG_BYTES) == PLACED)
        {
            // next tile is blocked
            return;
        }
        // replace current tile with carpet
        next_tile = (CARPET_1  | (player_x + player_y & 0b00000001));
        if ((tile & BG_BYTES) == PLACED)
        {
            // was placed - replace current tile with target
            next_tile = next_tile | TG_BYTE;
        }
        set_map_tile(player_x, player_y - 1, next_tile);
        is_player_pushing = 1;           
    }

    player_draw_left();
    // animate left
    map_frame = 1;        
    map_draw_horizontal();
    player_draw_background_horizontal();
    refresh_screen();
    map_frame++;
    map_draw_horizontal();
    player_draw_background_horizontal();
    refresh_screen();
    map_frame++;
    map_draw_horizontal();
    player_draw_background_horizontal();
    refresh_screen();
    player_y--;
    map_frame = 0;
    player_see(); // final position    
    map_draw_horizontal();
    player_draw_background_horizontal();
    if (is_player_pushing == 1) {
        // place BLOCK
        is_player_pushing = 0;
        unsigned char next_tile = get_map_tile(player_x, player_y - 1);
        if ((next_tile & TG_BYTE) == TG_BYTE) {
            // target location
            set_map_tile(player_x, player_y - 1, PLACED);
            // remove BLOCK as part of player
            map_draw_horizontal();
            player_draw_background_horizontal();
        } else {
            set_map_tile(player_x, player_y - 1, BLOCK);
        }
    }
    refresh_screen();
}

void map_move_right(void)
{
    unsigned char tile = get_map_tile(player_x, player_y + 1);
    if ((tile & BG_BYTES) == WALL)
    {
        // tile is inaccessible
        return;
    }

    if ((tile & BG_BYTES) == BLOCK || (tile & BG_BYTES) == PLACED)
    {
        // tile is a BLOCK or placed BLOCK
        unsigned char next_tile = get_map_tile(player_x, player_y + 2);
        if ((next_tile & BG_BYTES) == WALL || (next_tile & BG_BYTES) == BLOCK || (next_tile & BG_BYTES) == PLACED)
        {
            // next tile is blocked
            return;
        }
        // replace current tile with carpet
        next_tile = (CARPET_1  | (player_x + player_y & 0b00000001));
        if ((tile & BG_BYTES) == PLACED)
        {
            // was placed - replace current tile with target
            next_tile = next_tile | TG_BYTE;
        }
        set_map_tile(player_x, player_y + 1, next_tile);
        is_player_pushing = 1;            
    }

    player_draw_right();
    // animate right     
    map_frame = 3;        
    player_y++;
    map_draw_horizontal();
    player_draw_background_horizontal();
    refresh_screen();
    map_frame--;
    map_draw_horizontal();
    player_draw_background_horizontal();
    refresh_screen();
    map_frame--;
    map_draw_horizontal();
    player_draw_background_horizontal();
    refresh_screen();
    map_frame--;
    player_see(); // final position
    map_draw_horizontal();
    player_draw_background_horizontal();
    if (is_player_pushing == 1) {
        // place BLOCK
        is_player_pushing = 0;
        unsigned char next_tile = get_map_tile(player_x, player_y + 1);
        if ((next_tile & TG_BYTE) == TG_BYTE) {
            // target location
            set_map_tile(player_x, player_y + 1, PLACED);
            // remove BLOCK as part of player
            map_draw_horizontal();
            player_draw_background_horizontal();
        } else {
            set_map_tile(player_x, player_y + 1, BLOCK);
        } 
    }
    refresh_screen();
}