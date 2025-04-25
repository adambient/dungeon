#ifndef _INT_H
#define _INT_H

#define clock(x) (int_tick)

extern unsigned char int_tick;
extern unsigned char int_outer_tick;
extern unsigned char int_colour;

extern void int_refresh_screen(void);
extern void int_init(void);

#endif