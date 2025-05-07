#ifndef _GLOBALS_H
#define _GLOBALS_H

#define MAP_SIZE 16                        // 64x64 map
#define VISIBLE_BLOCKS 9                   // 11x11 displayed (must be odd number 3-11)
#define VISIBLE_AREA (VISIBLE_BLOCKS * 2)  // each block 2x2
#define PLAYER_SQUARE (VISIBLE_BLOCKS - 1) // position of player on screen
#define MAP_OFFSET (PLAYER_SQUARE / 2)     // offset to centralise display

#define BG_BYTES 0b00000111 // bytes used for background
#define SEEN_BYTE 0b0001000   // TODO - each square has a free bit which has been used for "seen" squares but we could/should think of something else
#define BLACK 0
#define BLUE 1
#define RED 2
#define MAGENTA 3
#define GREEN 4
#define CYAN 5
#define YELLOW 6
#define WHITE 7

// using these constants will allow us to experiment with colours
#define DARKNESS BLACK
#define CARPET_1 RED
#define CARPET_2 MAGENTA // this will actually always be CARPET_1 plus 1
#define ENEMY GREEN
#define WALL BLUE
#define CRATE YELLOW
#define PLACED GREEN
#define TARGET CYAN
#define PLAYER_FACE YELLOW
#define PLAYER_BODY_1 WHITE
#define PLAYER_BODY_2 BLACK

// player directions
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_NONE 4

extern unsigned char player_tile;
extern unsigned char player_x;
extern unsigned char player_y;
extern unsigned char player_dir;        // the way the player is currently moving 0-4
extern unsigned char player_facing;     // the way the player is currently facing 0-4
extern unsigned char is_player_pushing;
extern unsigned char is_player_pulling;
extern unsigned char display_attrs;     // whether to display the colour attrs or b&w

#endif