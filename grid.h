#ifndef _GRID_H
#define _GRID_H

// args for subroutines
typedef struct {
    unsigned char x;
    unsigned char y;
    unsigned char tile;
} grid_p;

extern grid_p grid_param;

// imported from grid.asm
extern unsigned char get_grid_tile(grid_p *p) __z88dk_fastcall;
extern void set_grid_tile(grid_p *p) __z88dk_fastcall;
// inline replacement
static inline unsigned char get_grid_tile_inline(unsigned char x, unsigned char y)
{
    grid_param.x = x;
    grid_param.y = y;
    return get_grid_tile(&grid_param);
}
// inline replacement
static inline void set_grid_tile_inline(unsigned char x, unsigned char y, unsigned char tile)
{
    grid_param.x = x;
    grid_param.y = y;
    grid_param.tile = tile;
    set_grid_tile(&grid_param);
}

#endif