#ifndef _MAP_H
#define _MAP_H

extern unsigned char map_uncovered_holes;
extern void map_init(void);
extern unsigned char map_move_up(void);
extern unsigned char map_move_down(void);
extern unsigned char map_move_left(void);
extern unsigned char map_move_right(void);
extern void map_move_none(void);

#endif