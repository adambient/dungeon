#include "sfx/tracker.h"

// imported from clotho.asm
extern unsigned char *clotho_channel1_score; // clotho channel 1 score
extern unsigned char *clotho_channel2_score; // clotho channel 2 score
extern unsigned char *clotho_channel3_score; // clotho channel 3 score
extern unsigned char *clotho_score; // clotho tracker score
extern unsigned char *clotho_score_end; // clotho tracker score

void music_select_clotho(void)
{
    tracker.channel1_start = (&clotho_channel1_score - 1); // -1 byte as first new note moves into position
    tracker.channel2_start = (&clotho_channel2_score - 1);
    tracker.channel3_start = (&clotho_channel3_score - 1);
    tracker.start = &clotho_score;
    tracker.note = &clotho_score_end; // reset tune
}