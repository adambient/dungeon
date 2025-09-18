#ifndef _SCREEN_H
#define _SCREEN_H

typedef struct {
    unsigned char colour_cycle;
    unsigned char colour;
} screen_t;

extern screen_t screen;

extern void screen_init(void);
extern void screen_refresh(void);
extern void screen_success(void);

#endif