#include "sound.h"

#define FOOTSTEP 0b00000001
#define PUSHING 0b00000010
#define SUCCESS 0b00000100

static unsigned char sound;
void play_footstep(void)
{
    sound = sound | FOOTSTEP;
}
void play_pushing(void)
{
    sound = sound | PUSHING;
}
void play_success(void)
{
    sound = sound | SUCCESS;
}
void play_sounds(void)
{
    // we can only play one sound at a time but there are many triggers so check in priority order
    if ((sound & SUCCESS) == SUCCESS)
    {
        bit_click();
    }
    else if ((sound & PUSHING) == PUSHING)
    {
        bit_click();
    }
    else if ((sound & FOOTSTEP) == FOOTSTEP)
    {
        bit_click();
    }
    sound = 0;
}