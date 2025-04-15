#include "globals.h"
#include "int.h"

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
extern void set_map_tile(unsigned char x, unsigned char y, unsigned int tile)  __z88dk_callee;
// imported from fill_rectangle.asm
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;
extern void bright_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width) __z88dk_callee;

unsigned char player_frame = 1;
unsigned char player_background_data; // lower bytes: player_background_1; higher bytes: player_background_2
unsigned char player_tile_data; // lower bytes: player_tile; higher bytes: player_tile_next

static inline unsigned char player_get_tile(unsigned char x, unsigned char y)
{
    return (get_map_tile(x, y) & BG_BYTES);   
}

static inline void set_player_tile(unsigned char player_tile)
{
    player_tile_data = ((player_tile_data & 0b11110000) | (player_tile & 0b00001111));
}

static inline unsigned char get_player_tile(void)
{
    return player_tile_data & 0b00001111;
}

static inline void set_player_tile_next(unsigned char player_tile_next)
{
    player_tile_data = ((player_tile_data & 0b00001111) | ((player_tile_next << 4) & 0b11110000));
}

static inline unsigned char get_player_tile_next(void)
{
    return (player_tile_data >> 4) & 0b00001111;
}

static inline void set_player_background_1(unsigned char player_background_1)
{
    player_background_data = ((player_background_data & 0b11110000) | (player_background_1 & 0b00001111));
}

static inline unsigned char get_player_background_1(void)
{
    return player_background_data & 0b00001111;
}

static inline void set_player_background_2(unsigned char player_background_2)
{
    player_background_data = ((player_background_data & 0b00001111) | ((player_background_2 << 4) & 0b11110000));
}

static inline unsigned char get_player_background_2(void)
{
    return (player_background_data >> 4) & 0b00001111;
}

static inline void frame_draw_up(void)
{        
    switch (player_frame)
    {
        default:
        case 1:
        case 3:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_UP_BODY1); // draw man
            player_frame++;
            break;
        case 2:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_UP_BODY2); // draw man
            player_frame++;
            break;
        case 4:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_UP_BODY3); // draw man
            player_frame = 1;
            break;   
    }
}

static inline void frame_draw_down(void)
{    
    switch (player_frame)
    {
        default:
        case 1:
        case 3:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_DOWN_BODY1); // draw man
            player_frame++;
            break;
        case 2:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_DOWN_BODY2); // draw man
            player_frame++;
            break;
        case 4:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_DOWN_BODY3); // draw man
            player_frame = 1;
            break;
    }
}

static inline void frame_draw_left(void)
{    
    switch (player_frame)
    {
        default:
        case 1:
        case 3:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_LEFT_BODY1); // draw man
            player_frame++;
            break;
        case 2:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_LEFT_BODY2); // draw man
            player_frame++;
            break;
        case 4:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_LEFT_BODY3); // draw man
            player_frame = 1;
            break;
    }
}

static inline void frame_draw_right(void)
{    
    switch (player_frame)
    {
        default:
        case 1:
        case 3:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_RIGHT_BODY1); // draw man
            player_frame++;
            break;
        case 2:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_RIGHT_BODY2); // draw man
            player_frame++;
            break;
        case 4:
            fill_rectangle_char(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 2, MAN_RIGHT_BODY3); // draw man
            player_frame = 1;
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
    set_player_tile(get_player_tile_next());
    set_player_tile_next(player_get_tile(player_x - 1, player_y));
    set_player_background_1(get_player_tile());
    set_player_background_2(get_player_tile_next());
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
    set_player_tile(get_player_tile_next());
    set_player_tile_next(player_get_tile(player_x, player_y + 1));
    set_player_background_1(get_player_tile());
    set_player_background_2(get_player_tile_next());
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
    set_player_tile(get_player_tile_next());
    set_player_tile_next(player_get_tile(player_x + 1, player_y));
    set_player_background_1(get_player_tile_next());
    set_player_background_2(get_player_tile());
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
    set_player_tile(get_player_tile_next());
    set_player_tile_next(player_get_tile(player_x, player_y - 1));
    set_player_background_1(get_player_tile_next());
    set_player_background_2(get_player_tile());
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
    unsigned char pb_1 = get_player_background_1();
    unsigned char pb_2 = get_player_background_2();
    if (pb_1 == TARGET) {
        pb_1 = colour;
    }
    if (pb_2 == TARGET) {
        pb_2 = colour;
    }
    if (player_facing == DIR_UP)
    {       
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 1, pb_2, PLAYER_BODY_1);
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE + 1, 1, 1, pb_2, PLAYER_BODY_2);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_BODY_2);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, pb_1, PLAYER_BODY_1);

        block_loc = PLAYER_SQUARE - 2;
    }
    else
    {
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, pb_2, PLAYER_FACE);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_BODY_1);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, pb_1, PLAYER_BODY_2);

        block_loc = PLAYER_SQUARE + 2;        
    }

    if (is_player_pushing) {
        // draw block next to player
        fill_rectangle_attr(block_loc, PLAYER_SQUARE, 2, 2, BLOCK, BLOCK);
    }

    // draw torchlight
    bright_rectangle_attr(PLAYER_SQUARE - 1, PLAYER_SQUARE - 1, 4, 4);
}

void player_draw_background_horizontal(void)
{
    unsigned char block_loc;
    // use cycled colour for target squares, store in temp var so next check works
    unsigned char pb_1 = get_player_background_1();
    unsigned char pb_2 = get_player_background_2();
    if (pb_1 == TARGET) {
        pb_1 = colour;
    }
    if (pb_2 == TARGET) {
        pb_2 = colour;
    }
    if (player_facing == DIR_LEFT)
    {       
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_FACE);
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE + 1, 1, 1, pb_2, PLAYER_BODY_1);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_BODY_2);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, pb_2, PLAYER_BODY_2);

        block_loc = PLAYER_SQUARE - 2;
    }
    else
    {
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_BODY_2);
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE + 1, 1, 1, pb_2, PLAYER_FACE);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, pb_1, PLAYER_BODY_1);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, pb_2, PLAYER_BODY_1);

        block_loc = PLAYER_SQUARE + 2;        
    }

    if (is_player_pushing) {
        // draw block next to player
        fill_rectangle_attr(PLAYER_SQUARE, block_loc, 2, 2, BLOCK, BLOCK);
    }

    // draw torchlight
    bright_rectangle_attr(PLAYER_SQUARE - 1, PLAYER_SQUARE - 1, 4, 4);
}

void player_draw_done(void)
{
    // final background draw should have solid background
    set_player_background_1(get_player_tile_next());
    set_player_background_2(get_player_tile_next());
}