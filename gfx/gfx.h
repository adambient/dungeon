#ifndef _gfx_H
#define _gfx_H

#define ATTR_BUFF 0xFC00 //768 bytes at the top of RAM

typedef struct {
    unsigned char y;
    unsigned char x;
    unsigned char width;
    unsigned char height;
    unsigned char paper;
    unsigned char ink;
    unsigned const char *c;
    unsigned const char *bank; // which character bank to use
} gfx_t;

extern gfx_t gfx;

extern const unsigned char *gfx_bank0; // default character bank

extern void gfx_char(void) __z88dk_callee;
extern void gfx_attr(void) __z88dk_callee;
extern void gfx_bright(void) __z88dk_callee;
extern void gfx_flush(void) __z88dk_callee;

#endif