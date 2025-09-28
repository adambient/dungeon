#include "beeps.h"
#include "sound.h"
#include "globals.h"

#define BEEPS_FOOTSTEP 0b00000001
#define BEEPS_PUSHING 0b00000010
#define BEEPS_SUCCESS 0b00000100

static unsigned char beeps_next;

void beeps_footstep(void)
{
    beeps_next = beeps_next | BEEPS_FOOTSTEP;
}
void beeps_pushing(void)
{
    beeps_next = beeps_next | BEEPS_PUSHING;
}
void beeps_success(void)
{
    beeps_next = beeps_next | BEEPS_SUCCESS;
}

void beeps_play(void)
{
    // we can only play one sound at a time but there are many triggers so check in priority order
    if ((beeps_next & BEEPS_SUCCESS) == BEEPS_SUCCESS)
    {
        bit_beepfx(BEEPFX_SWITCH_2);
    }
    else if ((beeps_next & BEEPS_PUSHING) == BEEPS_PUSHING)
    {
        bit_beepfx(BEEPFX_SWITCH_1);
    }
    else if ((beeps_next & BEEPS_FOOTSTEP) == BEEPS_FOOTSTEP)
    {
        bit_click();
    }
    beeps_next = 0;
}
void beeps_death(void)
{
    bit_beepfx(BEEPFX_POWER_OFF);
}
void beeps_winner(void)
{
    bit_beepfx(BEEPFX_SELECT_6);
}