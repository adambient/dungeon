#ifndef _INT_H
#define _INT_H

#ifndef WFRAMES
#define WFRAMES 2
#endif

#define clock(x) (tick)

extern unsigned char tick;
extern unsigned char colour;

extern void int_refresh_screen(void);
extern void int_wait(void);
extern void int_init(void);

#endif