#ifndef _sfx_H
#define _sfx_H

extern unsigned char sfx_note_wait; // ticks between notes
extern unsigned char *sfx_channel1_note; // next channel 1 note to play
extern unsigned char *sfx_channel2_note; // next channel 2 note to play
extern unsigned char *sfx_channel3_note; // next channel 3 note to play
extern unsigned char *sfx_note; // next tracker entry
extern unsigned char *sfx_channel1_start; // current tune channel 1 start
extern unsigned char *sfx_channel2_start; // current tune channel 2 start
extern unsigned char *sfx_channel3_start; // current tune channel 3 start
extern unsigned char *sfx_start; // current tune tracker start
extern unsigned const char *sfx_channel1_score; // clotho channel 1 score
extern unsigned const char *sfx_channel2_score; // clotho channel 2 score
extern unsigned const char *sfx_channel3_score; // clotho channel 3 score
extern unsigned const char *sfx_score; // clotho tracker score

extern void sfx_init(void) __z88dk_callee;
extern void sfx_play(void) __z88dk_callee;

#endif