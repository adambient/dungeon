#include "globals.h"
#include "screen.h"
#include "enemy.h"
#include "gfx.h"
#include "grid.h"

// UDGs
#define MAN_UP_HEAD "KL$"
#define MAN_UP_BODY1 "UV$"
#define MAN_UP_BODY2 "WX$"
#define MAN_UP_BODY3 "YZ$"
#define MAN_DOWN_HEAD "MN$"
#define MAN_DOWN_BODY1 "UV$"
#define MAN_DOWN_BODY2 "WX$"
#define MAN_DOWN_BODY3 "YZ$"
#define MAN_LEFT_HEAD "ML$"
#define MAN_LEFT_BODY1 "OP$"
#define MAN_LEFT_BODY2 "ST$"
#define MAN_LEFT_BODY3 "QR$"
#define MAN_RIGHT_HEAD "KN$"
#define MAN_RIGHT_BODY1 "OP$"
#define MAN_RIGHT_BODY2 "QR$"
#define MAN_RIGHT_BODY3 "ST$"

static unsigned char player_background_1;
static unsigned char player_background_2;
static unsigned char player_frame;
static unsigned char player_tile_next;

static unsigned char player_get_tile(unsigned char x, unsigned char y)
{
    if (enemy_is_located(x, y))
    {
        return ENEMY;
    }
    grid.x = x;
    grid.y = y;
    return (grid_get(&grid) & BG_BYTES);
}

static inline void frame_draw_body(unsigned const char *body1, unsigned const char *body2, unsigned const char *body3)
{
    // opt - deconstruct gfx_attr_inline to skip vars
    gfx.x = PLAYER_SQUARE + 1;
    gfx.y = PLAYER_SQUARE;
    gfx.height = 1;
    gfx.width = 2;
    switch (player_frame)
    {
    default:
    case 0:
        gfx.c = body1;
        player_frame = 1;
        break;
    case 1:
        gfx.c = body2;
        player_frame = 2;
        break;
    case 2:
        gfx.c = body1;
        player_frame = 3;
        break;
    case 3:
        gfx.c = body3;
        player_frame = 0;
        break;
    }
    gfx_char(&gfx);
}

// fills in all squares around the player
static inline void player_map_background(unsigned const char *background)
{
    // opt - deconstruct gfx_attr_inline to skip vars
    gfx.x = 0;
    gfx.y = 0;
    gfx.height = PLAYER_SQUARE + 2;
    gfx.width = PLAYER_SQUARE;
    gfx.c = background;
    gfx_char(&gfx);
    gfx.y = PLAYER_SQUARE;
    gfx.height = PLAYER_SQUARE;
    gfx.width = PLAYER_SQUARE + 2;
    gfx_char(&gfx);
    gfx.x = PLAYER_SQUARE + 2;
    gfx.y = 0;
    gfx.height = PLAYER_SQUARE;
    gfx_char(&gfx);
    gfx.x = PLAYER_SQUARE;
    gfx.y = PLAYER_SQUARE + 2;
    gfx.height = PLAYER_SQUARE + 2;
    gfx.width = PLAYER_SQUARE;
    gfx_char(&gfx);
}

void player_draw_up(void)
{
    player_tile = player_tile_next;
    player_tile_next = player_get_tile(player_x - 1, player_y);
    player_background_1 = player_tile;
    player_background_2 = player_tile_next;
    if (DIR_UP != player_facing)
    {
        switch (player_facing)
        {
        case DIR_RIGHT:
        case DIR_LEFT:
            player_map_background(BAR_PATTERN);
            break;
        }
        player_facing = DIR_UP;
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 2;
        gfx.c = MAN_UP_HEAD;
        gfx_char(&gfx);
    }
    frame_draw_body(MAN_UP_BODY1, MAN_UP_BODY2, MAN_UP_BODY3);
}

void player_draw_right(void)
{
    player_tile = player_tile_next;
    player_tile_next = player_get_tile(player_x, player_y + 1);
    player_background_1 = player_tile;
    player_background_2 = player_tile_next;
    if (DIR_RIGHT != player_facing)
    {
        switch (player_facing)
        {
        case DIR_UP:
        case DIR_DOWN:
            player_map_background(PIPE_PATTERN);
            break;
        }
        player_facing = DIR_RIGHT;
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 2;
        gfx.c = MAN_RIGHT_HEAD;
        gfx_char(&gfx);
    }
    frame_draw_body(MAN_RIGHT_BODY1, MAN_RIGHT_BODY2, MAN_RIGHT_BODY3);
}

void player_draw_down(void)
{
    player_tile = player_tile_next;
    player_tile_next = player_get_tile(player_x + 1, player_y);
    player_background_1 = player_tile_next;
    player_background_2 = player_tile;
    if (DIR_DOWN != player_facing)
    {
        switch (player_facing)
        {
        case DIR_RIGHT:
        case DIR_LEFT:
            player_map_background(BAR_PATTERN);
            break;
        }
        player_facing = DIR_DOWN;
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 2;
        gfx.c = MAN_DOWN_HEAD;
        gfx_char(&gfx);
    }
    frame_draw_body(MAN_DOWN_BODY1, MAN_DOWN_BODY2, MAN_DOWN_BODY3);
}

void player_draw_left(void)
{
    player_tile = player_tile_next;
    player_tile_next = player_get_tile(player_x, player_y - 1);
    player_background_1 = player_tile_next;
    player_background_2 = player_tile;
    if (DIR_LEFT != player_facing)
    {
        switch (player_facing)
        {
        case DIR_UP:
        case DIR_DOWN:
            player_map_background(PIPE_PATTERN);
            break;
        }
        player_facing = DIR_LEFT;
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 2;
        gfx.c = MAN_LEFT_HEAD;
        gfx_char(&gfx);
    }
    frame_draw_body(MAN_LEFT_BODY1, MAN_LEFT_BODY2, MAN_LEFT_BODY3);
}

void player_draw_background_vertical(void)
{
    unsigned char block_loc;
    // use cycled colour for target squares, store in temp var so next check works
    unsigned char pb_1 = player_background_1;
    unsigned char pb_2 = player_background_2;
    if (pb_1 == TARGET)
    {
        pb_1 = screen_colour;
    }
    if (pb_2 == TARGET)
    {
        pb_2 = screen_colour;
    }
    if (player_facing == DIR_UP)
    {
        // opt - deconstruct gfx_attr_inline to skip vars
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = pb_2;
        gfx.paper = PLAYER_BODY_1;
        gfx_attr(&gfx);
        gfx.y = PLAYER_SQUARE + 1;
        gfx.paper = PLAYER_BODY_2;
        gfx_attr(&gfx);
        gfx.x = PLAYER_SQUARE + 1;
        gfx.y = PLAYER_SQUARE;
        gfx.ink = pb_1;
        gfx_attr(&gfx);
        gfx.y = PLAYER_SQUARE + 1;
        gfx.paper = PLAYER_BODY_1;
        gfx_attr(&gfx);
        if (is_player_pushing)
        {
            block_loc = PLAYER_SQUARE - 2;
        }
        else
        {
            block_loc = PLAYER_SQUARE + 2;
        }
    }
    else
    {
        // opt - deconstruct gfx_attr_inline to skip vars
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 2;
        gfx.ink = pb_2;
        gfx.paper = PLAYER_FACE;
        gfx_attr(&gfx);        
        gfx.x = PLAYER_SQUARE + 1;
        gfx.width = 1;
        gfx.ink = pb_1;
        gfx.paper = PLAYER_BODY_1;
        gfx_attr(&gfx);
        gfx.y = PLAYER_SQUARE + 1;
        gfx.width = 1;
        gfx.paper = PLAYER_BODY_2;
        gfx_attr(&gfx);
        if (is_player_pushing)
        {
            block_loc = PLAYER_SQUARE + 2;
        }
        else
        {
            block_loc = PLAYER_SQUARE - 2;
        }
    }

    if (is_player_pushing || is_player_pulling)
    {
        // draw crate next to player
        gfx.x = block_loc;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 2;
        gfx.width = 2;
        gfx.ink = CRATE;
        gfx.paper = CRATE;
        gfx_attr(&gfx);
    }

    // draw torchlight
    gfx.x = PLAYER_SQUARE - 1;
    gfx.y = PLAYER_SQUARE - 1;
    gfx.height = 4;
    gfx.width = 4;
    gfx_bright(&gfx);
}

void player_draw_background_horizontal(void)
{
    unsigned char block_loc;
    // use cycled colour for target squares, store in temp var so next check works
    unsigned char pb_1 = player_background_1;
    unsigned char pb_2 = player_background_2;
    if (pb_1 == TARGET)
    {
        pb_1 = screen_colour;
    }
    if (pb_2 == TARGET)
    {
        pb_2 = screen_colour;
    }
    if (player_facing == DIR_LEFT)
    {
        // opt - deconstruct gfx_attr_inline to skip vars
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = pb_1;
        gfx.paper = PLAYER_FACE;
        gfx_attr(&gfx);
        gfx.y = PLAYER_SQUARE + 1;        
        gfx.ink = pb_2;
        gfx.paper = PLAYER_BODY_1;
        gfx_attr(&gfx);
        gfx.x = PLAYER_SQUARE + 1;
        gfx.y = PLAYER_SQUARE;
        gfx.ink = pb_1;
        gfx.paper = PLAYER_BODY_2;
        gfx_attr(&gfx);
        gfx.y = PLAYER_SQUARE + 1;
        gfx.ink = pb_2;
        gfx_attr(&gfx);
        if (is_player_pushing)
        {
            block_loc = PLAYER_SQUARE - 2;
        }
        else
        {
            block_loc = PLAYER_SQUARE + 2;
        }
    }
    else
    {
        // opt - deconstruct gfx_attr_inline to skip vars
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = pb_1;
        gfx.paper = PLAYER_BODY_2;
        gfx_attr(&gfx);
        gfx.y = PLAYER_SQUARE + 1;
        gfx.ink = pb_2;
        gfx.paper = PLAYER_FACE;
        gfx_attr(&gfx);
        gfx.x = PLAYER_SQUARE + 1;
        gfx.y = PLAYER_SQUARE;
        gfx.ink = pb_1;
        gfx.paper = PLAYER_BODY_1;
        gfx_attr(&gfx);
        gfx.y = PLAYER_SQUARE + 1;
        gfx.ink = pb_2;
        gfx_attr(&gfx);
        if (is_player_pushing)
        {
            block_loc = PLAYER_SQUARE + 2;
        }
        else
        {
            block_loc = PLAYER_SQUARE - 2;
        }
    }

    if (is_player_pushing || is_player_pulling)
    {
        // draw crate next to player
        gfx.x = PLAYER_SQUARE;
        gfx.y = block_loc;
        gfx.height = 2;
        gfx.width = 2;
        gfx.ink = CRATE;
        gfx.paper = CRATE;
        gfx_attr(&gfx);
    }

    // draw torchlight
    gfx.x = PLAYER_SQUARE - 1;
    gfx.y = PLAYER_SQUARE - 1;
    gfx.height = 4;
    gfx.width = 4;
    gfx_bright(&gfx);
}

void player_draw_done(void)
{
    // mark area around player as seen
    for (grid.x = player_x + 2; grid.x >= player_x - 2 && grid.x < 255; grid.x--)
    {
        for (grid.y = player_y + 2; grid.y >= player_y - 2 && grid.y < 255; grid.y--)
        {
            grid.tile = grid_get(&grid) | SEEN_BYTE;
            grid_set(&grid);
        }
    }
    // reset all backgrounds and next tiles to current tile
    player_background_1 =
    player_background_2 =
    player_tile =
    player_tile_next =
    player_get_tile(player_x, player_y);
}