#define ATTR_BUFF 0xF800 // hard coded attribute buffer address
#define MAP_SIZE 16 // 64x64 map
#define VISIBLE_BLOCKS 9 // 11x11 displayed (must be odd number 3-11)
#define VISIBLE_AREA (VISIBLE_BLOCKS * 2) // each block 2x2
#define PLAYER_SQUARE (VISIBLE_BLOCKS - 1) // position of player on screen
#define MAP_OFFSET (PLAYER_SQUARE / 2) // offset to centralise display

#define BLACK 0
#define BLUE 1
#define RED 2
#define MAGENTA 3
#define GREEN 4
#define CYAN 5
#define YELLOW 6
#define WHITE 7

extern unsigned char player_x;
extern unsigned char player_y;
extern unsigned char player_dir; // 0 - up, 1 - right, 2 - down, 3 left
extern unsigned char player_tile;
extern unsigned char player_tile_next;