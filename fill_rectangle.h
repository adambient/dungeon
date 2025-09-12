#ifndef _FILL_RECTANGLE_H
#define _FILL_RECTANGLE_H

// args for subroutines
typedef struct {
    unsigned char y;
    unsigned char x;
    unsigned char width;
    unsigned char height;
    unsigned char paper;
    unsigned char ink;
    unsigned const char *c;
} fill_rectangle_p;

extern fill_rectangle_p fill_rectangle_param;

// imported from fill_rectangle.asm
extern void fill_rectangle_char(fill_rectangle_p *p) __z88dk_fastcall;
static inline void fill_rectangle_char_inline(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned const char *c)
{
    fill_rectangle_param.x = x;
    fill_rectangle_param.y = y;
    fill_rectangle_param.height = height;
    fill_rectangle_param.width = width;
    fill_rectangle_param.c = c;
    fill_rectangle_char(&fill_rectangle_param);
}
extern void fill_rectangle_attr(fill_rectangle_p *p) __z88dk_fastcall;
static inline void fill_rectangle_attr_inline(unsigned char x, unsigned char y, unsigned char height, unsigned char width, unsigned char ink, unsigned char paper)
{
    fill_rectangle_param.x = x;
    fill_rectangle_param.y = y;
    fill_rectangle_param.height = height;
    fill_rectangle_param.width = width;
    fill_rectangle_param.ink = ink;
    fill_rectangle_param.paper = paper;
    fill_rectangle_attr(&fill_rectangle_param);
}
extern void bright_rectangle_attr(fill_rectangle_p *p) __z88dk_fastcall;
static inline void bright_rectangle_attr_inline(unsigned char x, unsigned char y, unsigned char height, unsigned char width)
{
    fill_rectangle_param.x = x;
    fill_rectangle_param.y = y;
    fill_rectangle_param.height = height;
    fill_rectangle_param.width = width;
    bright_rectangle_attr(&fill_rectangle_param);
}
extern void copy_attr_buffer(void) __z88dk_callee; // copy attribute buffer into attribute memory
// gets the memory address of the attribute buffer so it can be populated incrementally which is faster than using the index
static inline unsigned char *fill_rectangle_buffer_address(void)
{
    return (unsigned char *)(0x8000);
}

#endif