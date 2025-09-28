#include "globals.h"
#include "screen.h"
#include "enemy.h"
#include "gfx/gfx.h"
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

typedef struct {
    unsigned char background_1;
    unsigned char background_2;
    unsigned char frame;
    unsigned char tile_next;
} player_t;

static player_t player;

static unsigned char player_get_tile(unsigned char x, unsigned char y)
{
    if (enemy_is_located(x, y))
    {
        return ENEMY;
    }
    grid.x = x;
    grid.y = y;
    grid_get();
    return (grid.tile & BG_BYTES);
}

static inline void frame_draw_body(unsigned const char *body1, unsigned const char *body2, unsigned const char *body3)
{
    // opt - deconstruct gfx_attr_inline to skip vars
    gfx.x = PLAYER_SQUARE + 1;
    gfx.y = PLAYER_SQUARE;
    gfx.height = 1;
    gfx.width = 2;
    switch (player.frame)
    {
    default:
    case 0:
        gfx.c = body1;
        player.frame = 1;
        break;
    case 1:
        gfx.c = body2;
        player.frame = 2;
        break;
    case 2:
        gfx.c = body1;
        player.frame = 3;
        break;
    case 3:
        gfx.c = body3;
        player.frame = 0;
        break;
    }
    gfx.bank = &gfx_bank0;
    gfx_char();
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
    gfx.bank = &gfx_bank0;
    gfx_char();
    gfx.y = PLAYER_SQUARE;
    gfx.height = PLAYER_SQUARE;
    gfx.width = PLAYER_SQUARE + 2;
    gfx_char();
    gfx.x = PLAYER_SQUARE + 2;
    gfx.y = 0;
    gfx.height = PLAYER_SQUARE;
    gfx_char();
    gfx.x = PLAYER_SQUARE;
    gfx.y = PLAYER_SQUARE + 2;
    gfx.height = PLAYER_SQUARE + 2;
    gfx.width = PLAYER_SQUARE;
    gfx_char();
}

void player_draw_up(void)
{
    globals.player_tile = player.tile_next;
    player.tile_next = player_get_tile(globals.player_x - 1, globals.player_y);
    player.background_1 = globals.player_tile;
    player.background_2 = player.tile_next;
    if (DIR_UP != globals.player_facing)
    {
        switch (globals.player_facing)
        {
        case DIR_RIGHT:
        case DIR_LEFT:
            player_map_background(BAR_PATTERN);
            break;
        }
        globals.player_facing = DIR_UP;
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 2;
        gfx.c = MAN_UP_HEAD;
        gfx.bank = &gfx_bank0;
        gfx_char();
    }
    frame_draw_body(MAN_UP_BODY1, MAN_UP_BODY2, MAN_UP_BODY3);
}

void player_draw_right(void)
{
    globals.player_tile = player.tile_next;
    player.tile_next = player_get_tile(globals.player_x, globals.player_y + 1);
    player.background_1 = globals.player_tile;
    player.background_2 = player.tile_next;
    if (DIR_RIGHT != globals.player_facing)
    {
        switch (globals.player_facing)
        {
        case DIR_UP:
        case DIR_DOWN:
            player_map_background(PIPE_PATTERN);
            break;
        }
        globals.player_facing = DIR_RIGHT;
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 2;
        gfx.c = MAN_RIGHT_HEAD;
        gfx.bank = &gfx_bank0;
        gfx_char();
    }
    frame_draw_body(MAN_RIGHT_BODY1, MAN_RIGHT_BODY2, MAN_RIGHT_BODY3);
}

void player_draw_down(void)
{
    globals.player_tile = player.tile_next;
    player.tile_next = player_get_tile(globals.player_x + 1, globals.player_y);
    player.background_1 = player.tile_next;
    player.background_2 = globals.player_tile;
    if (DIR_DOWN != globals.player_facing)
    {
        switch (globals.player_facing)
        {
        case DIR_RIGHT:
        case DIR_LEFT:
            player_map_background(BAR_PATTERN);
            break;
        }
        globals.player_facing = DIR_DOWN;
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 2;
        gfx.c = MAN_DOWN_HEAD;
        gfx.bank = &gfx_bank0;
        gfx_char();
    }
    frame_draw_body(MAN_DOWN_BODY1, MAN_DOWN_BODY2, MAN_DOWN_BODY3);
}

void player_draw_left(void)
{
    globals.player_tile = player.tile_next;
    player.tile_next = player_get_tile(globals.player_x, globals.player_y - 1);
    player.background_1 = player.tile_next;
    player.background_2 = globals.player_tile;
    if (DIR_LEFT != globals.player_facing)
    {
        switch (globals.player_facing)
        {
        case DIR_UP:
        case DIR_DOWN:
            player_map_background(PIPE_PATTERN);
            break;
        }
        globals.player_facing = DIR_LEFT;
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 2;
        gfx.c = MAN_LEFT_HEAD;
        gfx.bank = &gfx_bank0;
        gfx_char();
    }
    frame_draw_body(MAN_LEFT_BODY1, MAN_LEFT_BODY2, MAN_LEFT_BODY3);
}

void player_draw_background_vertical(void)
{
    unsigned char block_loc;
    // use cycled colour for target squares, store in temp var so next check works
    unsigned char pb_1 = player.background_1;
    unsigned char pb_2 = player.background_2;
    if (pb_1 == TARGET)
    {
        pb_1 = screen.colour;
    }
    if (pb_2 == TARGET)
    {
        pb_2 = screen.colour;
    }
    if (globals.player_facing == DIR_UP)
    {
        // opt - deconstruct gfx_attr_inline to skip vars
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = pb_2;
        gfx.paper = PLAYER_BODY_1;
        gfx_attr();
        gfx.y = PLAYER_SQUARE + 1;
        gfx.paper = PLAYER_BODY_2;
        gfx_attr();
        gfx.x = PLAYER_SQUARE + 1;
        gfx.y = PLAYER_SQUARE;
        gfx.ink = pb_1;
        gfx_attr();
        gfx.y = PLAYER_SQUARE + 1;
        gfx.paper = PLAYER_BODY_1;
        gfx_attr();
        if (globals.is_player_pushing)
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
        gfx_attr();        
        gfx.x = PLAYER_SQUARE + 1;
        gfx.width = 1;
        gfx.ink = pb_1;
        gfx.paper = PLAYER_BODY_1;
        gfx_attr();
        gfx.y = PLAYER_SQUARE + 1;
        gfx.width = 1;
        gfx.paper = PLAYER_BODY_2;
        gfx_attr();
        if (globals.is_player_pushing)
        {
            block_loc = PLAYER_SQUARE + 2;
        }
        else
        {
            block_loc = PLAYER_SQUARE - 2;
        }
    }

    if (globals.is_player_pushing || globals.is_player_pulling)
    {
        // draw crate next to player
        gfx.x = block_loc;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 2;
        gfx.width = 2;
        gfx.ink = CRATE;
        gfx.paper = CRATE;
        gfx_attr();
    }

    // draw torchlight
    gfx.x = PLAYER_SQUARE - 1;
    gfx.y = PLAYER_SQUARE - 1;
    gfx.height = 4;
    gfx.width = 4;
    gfx_bright();
}

void player_draw_background_horizontal(void)
{
    unsigned char block_loc;
    // use cycled colour for target squares, store in temp var so next check works
    unsigned char pb_1 = player.background_1;
    unsigned char pb_2 = player.background_2;
    if (pb_1 == TARGET)
    {
        pb_1 = screen.colour;
    }
    if (pb_2 == TARGET)
    {
        pb_2 = screen.colour;
    }
    if (globals.player_facing == DIR_LEFT)
    {
        // opt - deconstruct gfx_attr_inline to skip vars
        gfx.x = PLAYER_SQUARE;
        gfx.y = PLAYER_SQUARE;
        gfx.height = 1;
        gfx.width = 1;
        gfx.ink = pb_1;
        gfx.paper = PLAYER_FACE;
        gfx_attr();
        gfx.y = PLAYER_SQUARE + 1;        
        gfx.ink = pb_2;
        gfx.paper = PLAYER_BODY_1;
        gfx_attr();
        gfx.x = PLAYER_SQUARE + 1;
        gfx.y = PLAYER_SQUARE;
        gfx.ink = pb_1;
        gfx.paper = PLAYER_BODY_2;
        gfx_attr();
        gfx.y = PLAYER_SQUARE + 1;
        gfx.ink = pb_2;
        gfx_attr();
        if (globals.is_player_pushing)
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
        gfx_attr();
        gfx.y = PLAYER_SQUARE + 1;
        gfx.ink = pb_2;
        gfx.paper = PLAYER_FACE;
        gfx_attr();
        gfx.x = PLAYER_SQUARE + 1;
        gfx.y = PLAYER_SQUARE;
        gfx.ink = pb_1;
        gfx.paper = PLAYER_BODY_1;
        gfx_attr();
        gfx.y = PLAYER_SQUARE + 1;
        gfx.ink = pb_2;
        gfx_attr();
        if (globals.is_player_pushing)
        {
            block_loc = PLAYER_SQUARE + 2;
        }
        else
        {
            block_loc = PLAYER_SQUARE - 2;
        }
    }

    if (globals.is_player_pushing || globals.is_player_pulling)
    {
        // draw crate next to player
        gfx.x = PLAYER_SQUARE;
        gfx.y = block_loc;
        gfx.height = 2;
        gfx.width = 2;
        gfx.ink = CRATE;
        gfx.paper = CRATE;
        gfx_attr();
    }

    // draw torchlight
    gfx.x = PLAYER_SQUARE - 1;
    gfx.y = PLAYER_SQUARE - 1;
    gfx.height = 4;
    gfx.width = 4;
    gfx_bright();
}

void player_draw_done(void)
{
    // mark area around player as seen
    for (grid.x = globals.player_x + 2; grid.x >= globals.player_x - 2 && grid.x < 255; grid.x--)
    {
        for (grid.y = globals.player_y + 2; grid.y >= globals.player_y - 2 && grid.y < 255; grid.y--)
        {
            grid_get();
            grid.tile = grid.tile | SEEN_BYTE;
            grid_set();
        }
    }
    // reset all backgrounds and next tiles to current tile
    player.background_1 =
    player.background_2 =
    globals.player_tile =
    player.tile_next =
    player_get_tile(globals.player_x, globals.player_y);
}