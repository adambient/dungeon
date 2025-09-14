#ifndef _GRID_H
#define _GRID_H

// args for subroutines
typedef struct {
    unsigned char x;
    unsigned char y;
    unsigned char tile;
} grid_p;

extern grid_p grid;

// imported from grid.asm
extern unsigned char grid_get(grid_p *p) __z88dk_fastcall;
extern void grid_set(grid_p *p) __z88dk_fastcall;

#endif