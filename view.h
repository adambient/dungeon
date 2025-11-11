#ifndef _VIEW_H
#define _VIEW_H

// TODO - should be somewhere accessible by map.h and view.h (tile.h?)
extern unsigned char tile_get(unsigned char x, unsigned char y);
extern void view_update(void);
extern void view_draw(void);

#endif