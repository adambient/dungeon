#ifndef _INT_H
#define _INT_H

#ifndef WFRAMES
#define WFRAMES 2
#endif

#define clock(x) (tick)

extern unsigned char tick;
extern unsigned char colour;

extern void refresh_screen(void);
extern void wait(void);
extern void setup_int(void);

#endif