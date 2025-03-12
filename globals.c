#include "globals.h"

unsigned char player_x;
unsigned char player_y;
unsigned char player_dir;
unsigned char player_tile;
unsigned char player_tile_next;

// map integer correspondence to make make definition of map[64][64] more readable

#define A 1 // BLUE - wall
#define B 2 // RED - enemy
#define C 3 // MAGENTA - ?
#define D 4 // GREEN - grass
#define E 5 // CYAN - water
#define F 6 // YELLOW - gold
#define G 7 // WHITE - player