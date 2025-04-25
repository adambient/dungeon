#include <stdlib.h>
#include "globals.h"
#include "player.h"
#include "enemy.h"
#include "int.h"
#include "beeps.h"

// TODO - this is obviously hard coded to be [16][6] so perhaps we should generate this, also is it optimal space wise?
static unsigned char map[MAP_SIZE][(MAP_SIZE / 8) * 3] = {
    // walls                 // BLOCKs               // target squares
    {0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10000001, 0b10000001, 0b00000000, 0b00000000, 0b00000100, 0b00100000},
    {0b10101101, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10000000, 0b00000001, 0b00100000, 0b00000100, 0b00000100, 0b00100000},
    {0b10101101, 0b11010101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10001101, 0b10010001, 0b00100000, 0b00000100, 0b00000000, 0b00000000},
    {0b10100001, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10111111, 0b10000101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10100001, 0b11111101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10101101, 0b10000101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10001001, 0b10110001, 0b00100000, 0b00000100, 0b00000000, 0b00000000},
    {0b10101011, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10000000, 0b00000001, 0b00100000, 0b00000100, 0b00000100, 0b00100000},
    {0b10101101, 0b10110101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
    {0b10000001, 0b10000001, 0b00000000, 0b00000000, 0b00000100, 0b00100000},
    {0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
};

// imported from map.asm
extern unsigned char get_map_tile(unsigned char x, unsigned char y) __z88dk_callee;
extern void set_map_tile(unsigned char x, unsigned char y, unsigned int tile) __z88dk_callee;
// imported from fill_rectangle.asm
extern unsigned char *attr_buffer;
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;
extern void bright_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width) __z88dk_callee;

static unsigned char *attr_address;
static unsigned char map_frame;

// gets the memory address of the attribute buffer so it can be populated incrementally which is faster than using the index
static inline unsigned char *get_start_attr_address(void)
{
    return (unsigned char *)(&attr_buffer);
}

static inline unsigned char row_get_tile(unsigned char x, unsigned char y)
{
    if (x < MAP_SIZE && y < MAP_SIZE)
    {        
        unsigned char tile = get_map_tile(x, y);
        if ((tile & SEEN_BYTE) == SEEN_BYTE)
        {
            if (x == enemy_x && y == enemy_y)
            {
                return ENEMY;
            }
            tile = tile & BG_BYTES;
            switch (tile)
            {
            case TARGET:
                return int_colour;
            case PLACED:
                // cycle bettween yellow and white
                return (YELLOW | (int_colour & 0b00000001));
            default:
                return tile;
            }
        }
    }

    return DARKNESS;
}

static void row_draw_vertical(signed char x, signed char x2, unsigned char y)
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

static void row_draw_horizontal(signed char x, unsigned char y)
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

static void map_draw_vertical(void)
{
    attr_address = get_start_attr_address(); // reset shared attr_address
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

static void map_draw_horizontal(void)
{
    attr_address = get_start_attr_address(); // reset shared attr_address
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
    for (unsigned char x = MAP_SIZE - 1; x < 255; x--)
    {
        for (unsigned char y = (MAP_SIZE / 8) - 1; y < 255; y--)
        {
            unsigned char i = 0;
            for (unsigned char i = 7; i < 255; i--)
            {
                unsigned char tile = WALL; // wall
                if ((map[x][y + 2] << i & 0b10000000) == 0b10000000)
                {
                    // use crate data
                    tile = CRATE;
                }
                else
                {
                    // use map data
                    if ((map[x][y] << i & 0b10000000) == 0b00000000)
                    {
                        if (((x + i) & 0b00000001) == 0b00000001)
                        {
                            tile = CARPET_1; // carpet 1
                        }
                        else
                        {
                            tile = CARPET_2; // carpet 2
                        }
                    }
                    if ((map[x][y + 4] << i & 0b10000000) == 0b10000000)
                    {
                        tile = TARGET;
                    }
                }
                set_map_tile(x, i + (8 * y), tile);
            }
        }
    }
}

static unsigned char can_move_check(signed char dx, signed char dy)
{
    unsigned char tile = get_map_tile(player_x + dx, player_y + dy);
    if ((tile & BG_BYTES) == WALL)
    {
        // tile is inaccessible
        return 0;
    }

    if ((tile & BG_BYTES) == CRATE || (tile & BG_BYTES) == PLACED)
    {
        if (!is_player_pushing)
        {
            // not pushing so cannot move
            return 0;
        }
        // we are pushing so check next tile
        unsigned char next_tile = get_map_tile(player_x + dx + dx, player_y + dy + dy);
        if ((next_tile & BG_BYTES) == WALL || (next_tile & BG_BYTES) == CRATE || (next_tile & BG_BYTES) == PLACED)
        {
            // next tile is blocked so cannot move
            is_player_pushing = 0;
            return 0;
        }
        if ((tile & BG_BYTES) == PLACED)
        {
            // replace tile in front
            set_map_tile(player_x + dx, player_y + dy, TARGET | SEEN_BYTE);
        }
        else
        {
            // replace tile in front
            set_map_tile(player_x + dx, player_y + dy, (CARPET_1 | (player_x + player_y & 0b00000001)) | SEEN_BYTE);
        }
        is_player_pushing = 1;
        play_pushing();
        return 1;
    }
    else if (is_player_pushing)
    {
        // we're not pushing but we might be pulling
        tile = get_map_tile(player_x - dx, player_y - dy);
        is_player_pushing = 0;
        if ((tile & BG_BYTES) == CRATE || (tile & BG_BYTES) == PLACED)
        {
            // there is a crate behind
            if ((tile & BG_BYTES) == PLACED)
            {
                // replace tile behind
                set_map_tile(player_x - dx, player_y - dy, TARGET | SEEN_BYTE);
            }
            else
            {
                // replace tile behind
                set_map_tile(player_x - dx, player_y - dy, (CARPET_1 | (player_x + player_y & 0b00000001)) | SEEN_BYTE);
            }
            is_player_pulling = 1;
            play_pushing();
            return 1;
        }
    }
    play_footstep();
    return 1;
}

static void map_move_done(signed char dx, signed char dy)
{
    if (is_player_pulling)
    {
        is_player_pulling = 0;
        is_player_pushing = 1;
        // we are pulling not pushing
        dx = dx * -1;
        dy = dy * -1;
    }

    if (is_player_pushing)
    {
        // place crate
        is_player_pushing = 0;
        unsigned char next_tile = get_map_tile(player_x + dx, player_y + dy);
        if ((next_tile & BG_BYTES) == TARGET)
        {
            // target location
            set_map_tile(player_x + dx, player_y + dy, PLACED | SEEN_BYTE);
            play_success();
        }
        else
        {
            set_map_tile(player_x + dx, player_y + dy, CRATE | SEEN_BYTE);
        }
    }
}

static void map_refresh_vertical(void)
{
    map_draw_vertical();
    player_draw_background_vertical();
    int_refresh_screen();
}

static void map_refresh_horizontal(void)
{
    map_draw_horizontal();
    player_draw_background_horizontal();
    int_refresh_screen();
}

unsigned char map_move_up(void)
{
    if (player_x == 0 || !can_move_check(-1, 0))
    {
        return DIR_NONE;
    }

    player_draw_up();
    // animate up
    map_frame = 1;
    map_refresh_vertical();
    map_frame = 2;
    map_refresh_vertical();
    map_frame = 3;
    map_refresh_vertical();
    player_x--;
    map_frame = 0;
    player_draw_done(); // final position
    map_move_done(-1, 0);
    map_refresh_vertical();
    return DIR_UP;
}

unsigned char map_move_down(void)
{
    if (player_x == MAP_SIZE - 1 || !can_move_check(1, 0))
    {
        return DIR_NONE;
    }

    player_draw_down();
    // animate down
    map_frame = 3;
    player_x++;
    map_refresh_vertical();
    map_frame = 2;
    map_refresh_vertical();
    map_frame = 1;
    map_refresh_vertical();
    map_frame = 0;
    player_draw_done(); // final position
    map_move_done(1, 0);
    map_refresh_vertical();
    return DIR_DOWN;
}

unsigned char map_move_left(void)
{
    if (player_y == 0 || !can_move_check(0, -1))
    {
        return DIR_NONE;
    }

    player_draw_left();
    // animate left
    map_frame = 1;
    map_refresh_horizontal();
    map_frame = 2;
    map_refresh_horizontal();
    map_frame = 3;
    map_refresh_horizontal();
    player_y--;
    map_frame = 0;
    player_draw_done(); // final position
    map_move_done(0, -1);
    map_refresh_horizontal();
    return DIR_LEFT;
}

unsigned char map_move_right(void)
{
    if (player_y == MAP_SIZE - 1 || !can_move_check(0, 1))
    {
        return DIR_NONE;
    }

    player_draw_right();
    // animate right
    map_frame = 3;
    player_y++;
    map_refresh_horizontal();
    map_frame = 2;
    map_refresh_horizontal();
    map_frame = 1;
    map_refresh_horizontal();
    map_frame = 0;
    player_draw_done(); // final position
    map_move_done(0, 1);
    map_refresh_horizontal();
    return DIR_RIGHT;
}

void map_move_none(void)
{
    is_player_pushing = 0;
    player_draw_done();
    switch (player_facing)
    {
    default:
    case 0:
    case 2:
        map_refresh_vertical();        
        break;
    case 1:
    case 3:
        map_refresh_horizontal();
        break;
    }    
}