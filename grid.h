#ifndef _GRID_H
#define _GRID_H

typedef struct {
    unsigned char y;
    unsigned char x;
    unsigned char tile;
} grid_t;

extern grid_t grid;

// imported from grid.asm
extern void grid_get(void) __z88dk_callee;
extern void grid_set(void) __z88dk_callee;

#endif