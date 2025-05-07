#ifndef _ENEMY_H
#define _ENEMY_H

extern void enemy_init(void);
extern void enemy_add(unsigned char x, unsigned char y);
extern void enemy_move(void);
extern unsigned char enemy_is_located(unsigned char y, unsigned char x);

#endif