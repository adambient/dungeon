#include "banker.h"

void exec_far(void (*fn)(void), unsigned char far_bank)
{
    unsigned char current_bank = banker_get();
    banker_set(far_bank);
    fn();
    banker_set(current_bank);
}

void exec_far_asm(void (*fn)(void) __z88dk_callee, unsigned char far_bank)
{
    unsigned char current_bank = banker_get();
    banker_set(far_bank);
    fn();
    banker_set(current_bank);
}