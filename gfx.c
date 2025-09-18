#include "gfx.h"

extern gfx_t gfx;
extern const char *gfx_bank0; // default character bank

// imported from gfx.asm
extern void gfx_char(void) __z88dk_callee;
extern void gfx_attr(void) __z88dk_callee;
extern void gfx_bright(void) __z88dk_callee;
extern void gfx_flush(void) __z88dk_callee;