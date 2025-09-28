#include "globals.h"
#include "player.h"
#include "enemy.h"
#include "screen.h"
#include "sfx/beeps.h"
#include "gfx/gfx.h"
#include "grid.h"
#include "banker.h"

static unsigned char *attr_address;
static unsigned char map_frame;
unsigned char map_uncovered_holes;

static inline unsigned char row_get_tile(unsigned char x, unsigned char y)
{
    if (x < MAP_SIZE && y < MAP_SIZE)
    {  
        grid.x = x;
        grid.y = y;
        grid_get();
        if ((grid.tile & SEEN_BYTE) == SEEN_BYTE)
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
                    return (CARPET_1 | (screen.colour & 0b00000001));
                }
            }
            grid.tile = grid.tile & BG_BYTES;
            switch (grid.tile)
            {
            case TARGET:
                return screen.colour;
            case PLACED:
                // cycle bettween yellow and white
                return (YELLOW | (screen.colour & 0b00000001));
            default:
                return grid.tile;
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
    attr_address = (unsigned char *)(ATTR_BUFF); // reset shared attr_address
    unsigned char sub_frame = 0;
    switch (map_frame)
    {
    case 1:
    case 3:
        sub_frame++;
        break;
    }
    signed char x = globals.player_x - MAP_OFFSET - 1; // starting row (could be negative)
    unsigned char y = globals.player_y - MAP_OFFSET;
    switch (map_frame)
    {
    case 2:
    case 3:
        row_draw_vertical(x, x, y);
        break;
    }
    signed char rows = globals.player_x + MAP_OFFSET;
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
    attr_address = (unsigned char *)(ATTR_BUFF); // reset shared attr_address
    unsigned char y = globals.player_y - MAP_OFFSET;
    if (map_frame == 2)
    {
        y--;
    }
    for (signed char x = globals.player_x - MAP_OFFSET; x <= globals.player_x + MAP_OFFSET; x++)
    {
        row_draw_horizontal(x, y);
        row_draw_horizontal(x, y);
    }
}

static unsigned char can_move_check(signed char dx, signed char dy)
{
    grid.x = globals.player_x + dx;
    grid.y = globals.player_y + dy;
    grid_get();
    unsigned char tile = grid.tile;
    if ((tile & BG_BYTES) == WALL)
    {
        // tile is inaccessible
        return 0;
    }

    if ((tile & BG_BYTES) == CRATE || (tile & BG_BYTES) == PLACED)
    {
        if (!globals.is_player_pushing)
        {
            // not pushing so cannot move
            return 0;
        }
        // we are pushing so check next tile
        grid.x = grid.x + dx;
        grid.y = grid.y + dy;
        grid_get();
        grid.tile = grid.tile & BG_BYTES;
        if (grid.tile == WALL || grid.tile == CRATE || grid.tile == PLACED || enemy_is_located(globals.player_x + dx + dx, globals.player_y + dy + dy))
        {
            // next tile is blocked so cannot move
            globals.is_player_pushing = 0;
            return 0;
        }
        grid.x = grid.x - dx;
        grid.y = grid.y - dy;
        if ((tile & BG_BYTES) == PLACED)
        {
            // replace tile in front
            grid.tile = TARGET | SEEN_BYTE;
            map_uncovered_holes++;
        }
        else
        {
            // replace tile in front
            grid.tile = (CARPET_1 | (globals.player_x + globals.player_y & 0b00000001)) | SEEN_BYTE;
        }
        grid_set();
        globals.is_player_pushing = 1;
        exec_far(beeps_pushing, 4);
        return 1;
    }
    else if (globals.is_player_pushing)
    {
        // we're not pushing but we might be pulling
        grid.x = globals.player_x - dx;
        grid.y = globals.player_y - dy;
        grid_get();
        tile = grid.tile;
        globals.is_player_pushing = 0;
        if ((tile & BG_BYTES) == CRATE || (tile & BG_BYTES) == PLACED)
        {
            // there is a crate behind
            if ((tile & BG_BYTES) == PLACED)
            {
                // replace tile behind 
                grid.tile = TARGET | SEEN_BYTE;
                map_uncovered_holes++;
            }
            else
            {
                // replace tile behind
                grid.tile = (CARPET_1 | (globals.player_x + globals.player_y & 0b00000001)) | SEEN_BYTE;
            }
            grid_set();
            globals.is_player_pulling = 1;
            exec_far(beeps_pushing, 4);
            return 1;
        }
    }
    exec_far(beeps_footstep, 4);
    return 1;
}

static void map_move_done(signed char dx, signed char dy)
{
    if (globals.is_player_pulling)
    {
        globals.is_player_pulling = 0;
        globals.is_player_pushing = 1;
        // we are pulling not pushing
        dx = -dx;
        dy = -dy;
    }

    if (globals.is_player_pushing)
    {
        // place crate
        globals.is_player_pushing = 0;
        grid.x = globals.player_x + dx;
        grid.y = globals.player_y + dy;
        grid_get();
        if ((grid.tile & BG_BYTES) == TARGET)
        {
            // target location
            grid.tile = PLACED | SEEN_BYTE;
            exec_far(beeps_success, 4);
            map_uncovered_holes--;
        }
        else
        {
            grid.tile = CRATE | SEEN_BYTE;
        }
        grid_set();
    }
}

static void map_refresh_vertical(void)
{
    map_draw_vertical();
    player_draw_background_vertical();
    screen_refresh();
}

static void map_refresh_horizontal(void)
{
    map_draw_horizontal();
    player_draw_background_horizontal();
    screen_refresh();
}

unsigned char map_move_up(void)
{
    if (globals.player_x == 0 || !can_move_check(-1, 0))
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
    globals.player_x--;
    map_frame = 0;
    player_draw_done(); // final position
    map_move_done(-1, 0);
    map_refresh_vertical();
    return DIR_UP;
}

unsigned char map_move_down(void)
{
    if (globals.player_x == MAP_SIZE - 1 || !can_move_check(1, 0))
    {
        return DIR_NONE;
    }

    player_draw_down();
    // animate down
    map_frame = 3;
    globals.player_x++;
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
    if (globals.player_y == 0 || !can_move_check(0, -1))
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
    globals.player_y--;
    map_frame = 0;
    player_draw_done(); // final position
    map_move_done(0, -1);
    map_refresh_horizontal();
    return DIR_LEFT;
}

unsigned char map_move_right(void)
{
    if (globals.player_y == MAP_SIZE - 1 || !can_move_check(0, 1))
    {
        return DIR_NONE;
    }

    player_draw_right();
    // animate right
    map_frame = 3;
    globals.player_y++;
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
    globals.is_player_pushing = 0;
    player_draw_done();
    switch (globals.player_facing)
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