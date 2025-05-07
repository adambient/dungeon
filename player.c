#include "globals.h"
#include "screen.h"
#include "enemy.h"

#define MAN_UP_HEAD "KL"
#define MAN_UP_BODY1 "UV"
#define MAN_UP_BODY2 "WX"
#define MAN_UP_BODY3 "YZ"
#define MAN_DOWN_HEAD "MN"
#define MAN_DOWN_BODY1 "UV"
#define MAN_DOWN_BODY2 "WX"
#define MAN_DOWN_BODY3 "YZ"
#define MAN_LEFT_HEAD "ML"
#define MAN_LEFT_BODY1 "OP"
#define MAN_LEFT_BODY2 "ST"
#define MAN_LEFT_BODY3 "QR"
#define MAN_RIGHT_HEAD "KN"
#define MAN_RIGHT_BODY1 "OP"
#define MAN_RIGHT_BODY2 "QR"
#define MAN_RIGHT_BODY3 "ST"
#define PIPE_PATTERN "\\"
#define BAR_PATTERN "["

// imported from map.asm
extern unsigned char get_map_tile(unsigned char x, unsigned char y) __z88dk_callee;
extern void set_map_tile(unsigned char x, unsigned char y, unsigned int tile) __z88dk_callee;
// imported from fill_rectangle.asm
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;
extern void bright_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width) __z88dk_callee;

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
    return (get_map_tile(x, y) & BG_BYTES);
}

static inline void frame_draw_up(void)
{
    switch (player_frame)
    {
    default:
    case 0:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_UP_BODY1); // draw man
        player_frame = 1;
        break;
    case 1:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_UP_BODY2); // draw man
        player_frame = 2;
        break;
    case 2:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_UP_BODY1); // draw man
        player_frame = 3;
        break;
    case 3:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_UP_BODY3); // draw man
        player_frame = 0;
        break;
    }
}

static inline void frame_draw_down(void)
{
    switch (player_frame)
    {
    default:
    case 0:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_DOWN_BODY1); // draw man
        player_frame = 1;
        break;
    case 1:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_DOWN_BODY2); // draw man
        player_frame = 2;
        break;
    case 2:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_DOWN_BODY1); // draw man
        player_frame = 3;
        break;
    case 3:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_DOWN_BODY3); // draw man
        player_frame = 0;
        break;
    }
}

static inline void frame_draw_left(void)
{
    switch (player_frame)
    {
    default:
    case 0:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_LEFT_BODY1); // draw man
        player_frame = 1;
        break;
    case 1:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_LEFT_BODY2); // draw man
        player_frame = 2;
        break;
    case 2:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_LEFT_BODY1); // draw man
        player_frame = 3;
        break;
    case 3:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_LEFT_BODY3); // draw man
        player_frame = 0;
        break;
    }
}

static inline void frame_draw_right(void)
{
    switch (player_frame)
    {
    default:
    case 0:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_RIGHT_BODY1); // draw man
        player_frame = 1;
        break;
    case 1:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_RIGHT_BODY2); // draw man
        player_frame = 2;
        break;
    case 2:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_RIGHT_BODY1); // draw man
        player_frame = 3;
        break;

    case 3:
        fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_RIGHT_BODY3); // draw man
        player_frame = 0;
        break;
    }
}

static inline void player_map_bars(void)
{
    // fills in all squares around the player
    fill_rectangle_char(0, 0, PLAYER_SQUARE + 2, PLAYER_SQUARE, BAR_PATTERN);
    fill_rectangle_char(0, PLAYER_SQUARE, PLAYER_SQUARE, PLAYER_SQUARE + 2, BAR_PATTERN);
    fill_rectangle_char(PLAYER_SQUARE + 2, 0, PLAYER_SQUARE, PLAYER_SQUARE + 2, BAR_PATTERN);
    fill_rectangle_char(PLAYER_SQUARE, PLAYER_SQUARE + 2, PLAYER_SQUARE + 2, PLAYER_SQUARE, BAR_PATTERN);
}

static inline void player_map_pipes(void)
{
    // fills in all squares around the player
    fill_rectangle_char(0, 0, PLAYER_SQUARE + 2, PLAYER_SQUARE, PIPE_PATTERN);
    fill_rectangle_char(0, PLAYER_SQUARE, PLAYER_SQUARE, PLAYER_SQUARE + 2, PIPE_PATTERN);
    fill_rectangle_char(PLAYER_SQUARE + 2, 0, PLAYER_SQUARE, PLAYER_SQUARE + 2, PIPE_PATTERN);
    fill_rectangle_char(PLAYER_SQUARE, PLAYER_SQUARE + 2, PLAYER_SQUARE + 2, PLAYER_SQUARE, PIPE_PATTERN);
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
            player_map_bars();
            break;
        }
        player_facing = DIR_UP;
        fill_rectangle_char(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, MAN_UP_HEAD);
    }
    frame_draw_up();
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
            player_map_pipes();
            break;
        }
        player_facing = DIR_RIGHT;
        fill_rectangle_char(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, MAN_RIGHT_HEAD);
    }
    frame_draw_right();
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
            player_map_bars();
            break;
        }
        player_facing = DIR_DOWN;
        fill_rectangle_char(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, MAN_DOWN_HEAD);
    }
    frame_draw_down();
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
            player_map_pipes();
            break;
        }
        player_facing = DIR_LEFT;
        fill_rectangle_char(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, MAN_LEFT_HEAD);
    }
    frame_draw_left();
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
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 1, pb_2, PLAYER_BODY_1);
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE + 1, 1, 1, pb_2, PLAYER_BODY_2);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_BODY_2);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, pb_1, PLAYER_BODY_1);
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
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, pb_2, PLAYER_FACE);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_BODY_1);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, pb_1, PLAYER_BODY_2);
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
        fill_rectangle_attr(block_loc, PLAYER_SQUARE, 2, 2, CRATE, CRATE);
    }

    // draw torchlight
    bright_rectangle_attr(PLAYER_SQUARE - 1, PLAYER_SQUARE - 1, 4, 4);
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
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_FACE);
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE + 1, 1, 1, pb_2, PLAYER_BODY_1);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_BODY_2);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, pb_2, PLAYER_BODY_2);
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
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_BODY_2);
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE + 1, 1, 1, pb_2, PLAYER_FACE);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_BODY_1);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, pb_2, PLAYER_BODY_1);
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
        fill_rectangle_attr(PLAYER_SQUARE, block_loc, 2, 2, CRATE, CRATE);
    }

    // draw torchlight
    bright_rectangle_attr(PLAYER_SQUARE - 1, PLAYER_SQUARE - 1, 4, 4);
}

void player_draw_done(void)
{
    // mark area around player as seen
    for (unsigned char x = player_x + 2; x >= player_x - 2 && x < 255; x--)
    {
        for (unsigned char y = player_y + 2; y >= player_y - 2 && y < 255; y--)
        {
            set_map_tile(x, y, get_map_tile(x, y) | SEEN_BYTE);
        }
    }
    // reset all backgrounds and next tiles to current tile
    player_background_1 =
    player_background_2 =
    player_tile =
    player_tile_next =
    player_get_tile(player_x, player_y);
}