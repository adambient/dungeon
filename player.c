#include "globals.h"

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
#define DIR_UP 1
#define DIR_LEFT 2
#define DIR_DOWN 3
#define DIR_RIGHT 4

// imported from map.asm
extern unsigned char get_map_tile(unsigned char x, unsigned char y) __z88dk_callee;
extern void set_map_tile(unsigned char x, unsigned char y, unsigned int tile)  __z88dk_callee;
// imported from fill_rectangle.asm
extern void fill_rectangle_char(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char *c) __z88dk_callee;
extern void fill_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper) __z88dk_callee;
extern void bright_rectangle_attr(unsigned char x, unsigned char y, unsigned char height, unsigned char width) __z88dk_callee;
extern void copy_attr_buffer(void) __z88dk_callee; // copy attribute buffer into attribute memory

unsigned char player_frame = 1;
unsigned char player_direction = 0; // 1:up;2:right;3:down;4:left
unsigned char player_background_1;
unsigned char player_background_2;
unsigned char player_torch_size = 1;

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

static inline unsigned char player_get_tile(unsigned char x, unsigned char y)
{
    // no need to check if seen but rotate to last 3 (background)
    return (get_map_tile(x, y) >> 1 & 0b00000111);
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
    if (DIR_UP != player_direction)
    {        
        switch (player_direction)
        {
            case DIR_RIGHT:
            case DIR_LEFT:
                player_map_bars();
                break;
        }
        player_direction = DIR_UP;
        fill_rectangle_char(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, MAN_UP_HEAD);
    }
    player_tile = player_tile_next;
    player_tile_next = player_get_tile(player_x - 1, player_y);
    player_background_1 = player_tile;
    player_background_2 = player_tile_next;
    frame_draw_up();
}

void player_draw_right(void)
{
    if (DIR_RIGHT != player_direction)
    {        
        switch (player_direction)
        {
            case DIR_UP:
            case DIR_DOWN:            
                player_map_pipes();
                break;
        }
        player_direction = DIR_RIGHT;
        fill_rectangle_char(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, MAN_RIGHT_HEAD);
    }
    player_tile = player_tile_next;
    player_tile_next = player_get_tile(player_x, player_y + 1);
    player_background_1 = player_tile;
    player_background_2 = player_tile_next;    
    frame_draw_right();
}

void player_draw_down(void)
{
    if (DIR_DOWN != player_direction)
    {        
        switch (player_direction)
        {
            case DIR_RIGHT:
            case DIR_LEFT:
                player_map_bars();
                break;
        }
        player_direction = DIR_DOWN;
        fill_rectangle_char(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, MAN_DOWN_HEAD);
    }
    player_tile = player_tile_next;
    player_tile_next = player_get_tile(player_x + 1, player_y);
    player_background_1 = player_tile_next;
    player_background_2 = player_tile;    
    frame_draw_down();
}

void player_draw_left(void)
{
    if (DIR_LEFT != player_direction)
    {        
        switch (player_direction)
        {
            case DIR_UP:
            case DIR_DOWN:            
                player_map_pipes();
                break;
        }
        player_direction = DIR_LEFT;
        fill_rectangle_char(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, MAN_LEFT_HEAD);
    }
    player_tile = player_tile_next;
    player_tile_next = player_get_tile(player_x, player_y - 1);
    player_background_1 = player_tile_next;
    player_background_2 = player_tile;
    frame_draw_left();
}

void player_draw_background_vertical(void)
{
    if (player_direction == DIR_UP)
    {       
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 1, player_background_2, BLACK);
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE + 1, 1, 1, player_background_2, WHITE);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, player_background_1, WHITE);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, player_background_1, BLACK);
    }
    else
    {
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 2, player_background_2, YELLOW);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, player_background_1, BLACK);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, player_background_1, WHITE);
    }    
    bright_rectangle_attr(PLAYER_SQUARE - player_torch_size, PLAYER_SQUARE - player_torch_size, 2 + player_torch_size + player_torch_size, 2 + player_torch_size + player_torch_size);    
}

void player_draw_background_horizontal(void)
{
    if (player_direction == DIR_LEFT)
    {       
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 1, player_background_1, YELLOW);
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE + 1, 1, 1, player_background_2, BLACK);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, player_background_1, WHITE);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, player_background_2, WHITE);
    }
    else
    {
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE, 1, 1, player_background_1, WHITE);
        fill_rectangle_attr(PLAYER_SQUARE, PLAYER_SQUARE + 1, 1, 1, player_background_2, YELLOW);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE, 1, 1, player_background_1, BLACK);
        fill_rectangle_attr(PLAYER_SQUARE + 1, PLAYER_SQUARE + 1, 1, 1, player_background_2, BLACK);
    }          
    bright_rectangle_attr(PLAYER_SQUARE - player_torch_size, PLAYER_SQUARE - player_torch_size, 2 + player_torch_size + player_torch_size, 2 + player_torch_size + player_torch_size);
}

void player_see(unsigned char up, unsigned char down, unsigned char left, unsigned char right)
{
    // mark area around player as seen
    for (unsigned char x = player_x + down; x >= player_x - up && x < 255; x--)
    {
        for (unsigned char y = player_y + right; y >= player_y - left && y < 255; y--)
        {
            set_map_tile(x, y, get_map_tile(x, y) | 0b00000001);
        }
    }
    // toggle the torch size
    switch (player_torch_size)
    {
        default:
        case 1:
            player_torch_size = 2;
            break;
        case 2:
            player_torch_size = 1;
            break;
    }
    // final background draw should have solid background
    player_background_1 = player_tile_next;
    player_background_2 = player_tile_next;
}