#define MAP_SIZE 16 // 64x64 map
#define VISIBLE_BLOCKS 9 // 11x11 displayed (must be odd number 3-11)
#define VISIBLE_AREA (VISIBLE_BLOCKS * 2) // each block 2x2
#define PLAYER_SQUARE (VISIBLE_BLOCKS - 1) // position of player on screen
#define MAP_OFFSET (PLAYER_SQUARE / 2) // offset to centralise display

#define BG_BYTES 0b00001110
#define BLACK 0
#define BG_BLACK 0b00000000
#define BG_BLACK_SEEN 0b00000001
#define BLUE 1
#define BG_BLUE 0b00000010
#define BG_BLUE_SEEN 0b00000011
#define RED 2
#define BG_RED 0b00000100
#define BG_RED_SEEN 0b00000101
#define MAGENTA 3
#define BG_MAGENTA 0b00000110
#define BG_MAGENTA_SEEN 0b00000111
#define GREEN 4
#define BG_GREEN 0b00001000
#define BG_GREEN_SEEN 0b00001001
#define CYAN 5
#define BG_CYAN 0b00001010
#define BG_CYAN_SEEN 0b00001011
#define YELLOW 6
#define BG_YELLOW 0b00001100
#define BG_YELLOW_SEEN 0b00001101
#define WHITE 7
#define BG_WHITE 0b00001110
#define BG_WHITE_SEEN 0b00001111

extern unsigned char player_x;
extern unsigned char player_y;
extern unsigned char player_dir; // 0 - up, 1 - right, 2 - down, 3 left
extern unsigned char player_tile;
extern unsigned char player_tile_next;
extern unsigned char is_player_pushing; // TODO - use for state, not just is pushing