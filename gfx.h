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
} gfx_p;

extern gfx_p gfx;

// imported from gfx.asm
extern void gfx_char(gfx_p *p) __z88dk_fastcall;
extern void gfx_attr(gfx_p *p) __z88dk_fastcall;
extern void gfx_bright(gfx_p *p) __z88dk_fastcall;
extern void gfx_flush(void) __z88dk_callee; // copy attribute buffer into attribute memory
// gets the memory address of the attribute buffer so it can be populated incrementally which is faster than using the index
static inline unsigned char *gfx_buffer(void)
{
    return (unsigned char *)(0x8000);
}

#endif