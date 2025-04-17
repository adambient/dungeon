#include "globals.h"

unsigned char player_x;
unsigned char player_y;
unsigned char player_dir;
unsigned char player_facing = DIR_NONE;
static unsigned char player_state_data; // 0,0,0,0,0,0,is_player_pulling,is_player_pushing

void set_is_player_pushing(unsigned char is_player_pushing)
{
    player_state_data = ((player_state_data & 0b11111110) | (is_player_pushing & 0b00000001));
}

unsigned char get_is_player_pushing(void)
{
    return player_state_data & 0b00000001;
}

void set_is_player_pulling(unsigned char is_player_pulling)
{
    player_state_data = ((player_state_data & 0b11111101) | ((is_player_pulling << 1) & 0b00000010));
}

unsigned char get_is_player_pulling(void)
{
    return (player_state_data >> 1) & 0b00000001;
}