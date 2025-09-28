#ifndef _tracker_H
#define _tracker_H

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

extern void tracker_init(void) __z88dk_callee;
extern void tracker_play(void) __z88dk_callee;
extern void tracker_stop(void) __z88dk_callee;
extern void tracker_select_clotho(void);

#endif