#ifndef _tracker_H
#define _tracker_H

#include "banker.h"

typedef struct {
    unsigned char note_wait; // ticks between notes
    unsigned char *channel1_note; // next channel 1 note to play
    unsigned char *channel2_note; // next channel 2 note to play
    unsigned char *channel3_note; // next channel 3 note to play
    unsigned char *note; // next tracker entry
    unsigned char *channel1_start; // current tune channel 1 start
    unsigned char *channel2_start; // current tune channel 2 start
    unsigned char *channel3_start; // current tune channel 3 start
    unsigned char *start; // current tune tracker start
} tracker_t;

extern tracker_t tracker;

extern unsigned char *clotho_channel1_score; // clotho channel 1 score
extern unsigned char *clotho_channel2_score; // clotho channel 2 score
extern unsigned char *clotho_channel3_score; // clotho channel 3 score
extern unsigned char *clotho_score; // clotho tracker score
extern unsigned char *clotho_score_end; // clotho tracker score

extern void tracker_init(void) __z88dk_callee;
extern void tracker_play(void) __z88dk_callee;
extern void tracker_stop(void) __z88dk_callee;
extern void tracker_clotho(void) __z88dk_callee;

// TODO - this does not work, fix
static inline void tracker_clotho_inline(void)
{
    unsigned char current_bank = banker_get();
    banker_set(4);
    tracker.channel1_start = clotho_channel1_score - 2; // -2 as first new note moves into position
    tracker.channel2_start = clotho_channel2_score - 2;
    tracker.channel3_start = clotho_channel3_score - 2;
    tracker.start = clotho_score;
    tracker.note = clotho_score_end;
    banker_set(current_bank);
}

#endif