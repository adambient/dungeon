#ifndef BANKER_H
#define BANKER_H

extern unsigned int banker_get(void) __z88dk_fastcall;
extern void banker_set(unsigned char bank) __z88dk_fastcall;
extern void exec_far(void (*fn)(void), unsigned char far_bank);

#endif