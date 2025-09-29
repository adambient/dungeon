#include "sfx/tracker.h"

// imported from clotho.asm
extern unsigned char clotho_note_wait; // clotho note waut
extern unsigned char *clotho_channel1_score; // clotho channel 1 score
extern unsigned char *clotho_channel2_score; // clotho channel 2 score
extern unsigned char *clotho_channel3_score; // clotho channel 3 score
extern unsigned char *clotho_score; // clotho tracker score
extern unsigned char *clotho_score_end; // clotho tracker score end

// imported from hubbard.asm
extern unsigned char hubbard_note_wait; // hubbard note waut
extern unsigned char *hubbard_channel1_score; // hubbard channel 1 score
extern unsigned char *hubbard_channel2_score; // hubbard channel 2 score
extern unsigned char *hubbard_channel3_score; // hubbard channel 3 score
extern unsigned char *hubbard_score; // hubbard tracker score
extern unsigned char *hubbard_score_end; // hubbard tracker score end

void music_select_clotho(void)
{
    tracker.note_wait = clotho_note_wait;
    tracker.channel1_start = (&clotho_channel1_score - 1); // -1 (2 bytes) as first new note moves into position
    tracker.channel2_start = (&clotho_channel2_score - 1);
    tracker.channel3_start = (&clotho_channel3_score - 1);
    tracker.start = &clotho_score;
    tracker.note = &clotho_score_end; // reset tune
}

void music_select_hubbard(void)
{
    tracker.note_wait = hubbard_note_wait;
    tracker.channel1_start = (&hubbard_channel1_score - 1); // -1 (2 bytes) as first new note moves into position
    tracker.channel2_start = (&hubbard_channel2_score - 1);
    tracker.channel3_start = (&hubbard_channel3_score - 1);
    tracker.start = &hubbard_score;
    tracker.note = &hubbard_score_end; // reset tune
}