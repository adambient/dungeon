#include "sound.h"

#define FOOTSTEP 0b00000001
#define PUSHING 0b00000010
#define SUCCESS 0b00000100

static unsigned char sound;
void beeps_footstep(void)
{
    sound = sound | FOOTSTEP;
}
void beeps_pushing(void)
{
    sound = sound | PUSHING;
}
void beeps_success(void)
{
    sound = sound | SUCCESS;
}
void beeps_play(void)
{
    // we can only play one sound at a time but there are many triggers so check in priority order
    if ((sound & SUCCESS) == SUCCESS)
    {
        bit_beepfx(BEEPFX_SWITCH_2);
    }
    else if ((sound & PUSHING) == PUSHING)
    {
        bit_beepfx(BEEPFX_SWITCH_1);
    }
    else if ((sound & FOOTSTEP) == FOOTSTEP)
    {
        bit_click();
    }
    sound = 0;
}
void beeps_death(void)
{
    bit_beepfx(BEEPFX_POWER_OFF);
}
void beeps_winner(void)
{
    bit_beepfx(BEEPFX_SELECT_6);
}