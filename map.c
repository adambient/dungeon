#include "globals.h"
#include "player.h"
#include "enemy.h"
#include "screen.h"
#include "beeps.h"
#include "fill_rectangle.h"
#include "grid.h"

static unsigned char *attr_address;
static unsigned char map_frame;
unsigned char map_uncovered_holes;

static inline unsigned char row_get_tile(unsigned char x, unsigned char y)
{
    if (x < MAP_SIZE && y < MAP_SIZE)
    {  
        unsigned char tile = get_grid_tile_inline(x, y);
        if ((tile & SEEN_BYTE) == SEEN_BYTE)
        {
            if (enemy_is_located(x, y))
            {
                return ENEMY;
            }
            if (map_uncovered_holes == 0)
            {
                if ((x == 1 && (y == (MAP_SIZE - 2) || y == 1)) ||
                (x == (MAP_SIZE - 2) && (y == (MAP_SIZE - 2) || y == 1)))
                {
                    // uncovered secret exit, cycle between carpet colours
                    return (CARPET_1 | (screen_colour & 0b00000001));
                }
            }
            tile = tile & BG_BYTES;
            switch (tile)
            {
            case TARGET:
                return screen_colour;
            case PLACED:
                // cycle bettween yellow and white
                return (YELLOW | (screen_colour & 0b00000001));
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
    attr_address = fill_rectangle_buffer_address(); // reset shared attr_address
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
    attr_address = fill_rectangle_buffer_address(); // reset shared attr_address
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

static unsigned char can_move_check(signed char dx, signed char dy)
{
    unsigned char tile = get_grid_tile_inline(player_x + dx, player_y + dy);
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
        unsigned char next_tile = get_grid_tile_inline(player_x + dx + dx, player_y + dy + dy) & BG_BYTES;
        if (next_tile == WALL || next_tile == CRATE || next_tile == PLACED || enemy_is_located(player_x + dx + dx, player_y + dy + dy))
        {
            // next tile is blocked so cannot move
            is_player_pushing = 0;
            return 0;
        }
        if ((tile & BG_BYTES) == PLACED)
        {
            // replace tile in front
            set_grid_tile_inline(player_x + dx, player_y + dy, TARGET | SEEN_BYTE);
            map_uncovered_holes++;
        }
        else
        {
            // replace tile in front
            set_grid_tile_inline(player_x + dx, player_y + dy, (CARPET_1 | (player_x + player_y & 0b00000001)) | SEEN_BYTE);
        }
        is_player_pushing = 1;
        beeps_pushing();
        return 1;
    }
    else if (is_player_pushing)
    {
        // we're not pushing but we might be pulling
        tile = get_grid_tile_inline(player_x - dx, player_y - dy);
        is_player_pushing = 0;
        if ((tile & BG_BYTES) == CRATE || (tile & BG_BYTES) == PLACED)
        {
            // there is a crate behind
            if ((tile & BG_BYTES) == PLACED)
            {
                // replace tile behind                
                set_grid_tile_inline(player_x - dx, player_y - dy, TARGET | SEEN_BYTE);
                map_uncovered_holes++;
            }
            else
            {
                // replace tile behind
                set_grid_tile_inline(player_x - dx, player_y - dy, (CARPET_1 | (player_x + player_y & 0b00000001)) | SEEN_BYTE);
            }
            is_player_pulling = 1;
            beeps_pushing();
            return 1;
        }
    }
    beeps_footstep();
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
        unsigned char next_tile = get_grid_tile_inline(player_x + dx, player_y + dy);
        if ((next_tile & BG_BYTES) == TARGET)
        {
            // target location
            set_grid_tile_inline(player_x + dx, player_y + dy, PLACED | SEEN_BYTE);
            beeps_success();
            map_uncovered_holes--;
        }
        else
        {
            set_grid_tile_inline(player_x + dx, player_y + dy, CRATE | SEEN_BYTE);
        }
    }
}

static void map_refresh_vertical(void)
{
    if (display_attrs)
    {
        map_draw_vertical();
    }
    else
    {
        fill_rectangle_attr_inline(0, 0, VISIBLE_AREA, VISIBLE_AREA, 0, 7);
    }
    player_draw_background_vertical();
    screen_refresh();
}

static void map_refresh_horizontal(void)
{
    if (display_attrs)
    {
        map_draw_horizontal();
    }
    else
    {
        fill_rectangle_attr_inline(0, 0, VISIBLE_AREA, VISIBLE_AREA, 0, 7);
    }
    player_draw_background_horizontal();
    screen_refresh();
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