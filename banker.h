#ifndef BANKER_H
#define BANKER_H

extern void exec_far(void (*fn)(void), unsigned char far_bank);
extern void exec_far_arg(void (*fn)(unsigned int), unsigned int arg, unsigned char far_bank);

#endif