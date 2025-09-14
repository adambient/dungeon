#ifndef _gfx_H
#define _gfx_H

// args for subroutines
typedef struct {
    unsigned char y;
    unsigned char x;
    unsigned char width;
    unsigned char height;
    unsigned char paper;
    unsigned char ink;
    unsigned const char *c;
} gfx_t;

extern gfx_t gfx;

// imported from gfx.asm
extern void gfx_char(void) __z88dk_callee;
extern void gfx_attr(void) __z88dk_callee;
extern void gfx_bright(void) __z88dk_callee;
extern void gfx_flush(void) __z88dk_callee;

#endif