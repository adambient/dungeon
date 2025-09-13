// imported from banker.asm
extern unsigned int pager_get(void) __z88dk_fastcall;
extern void pager_set(unsigned char bank) __z88dk_fastcall;

void exec_far(void (*fn)(void), unsigned char far_bank)
{
    unsigned char current_bank = pager_get();
    pager_set(far_bank);
    fn();
    pager_set(current_bank);
}

void exec_far_arg(void (*fn)(unsigned int), unsigned int arg, unsigned char far_bank)
{
    unsigned char current_bank = pager_get();
    pager_set(far_bank);
    fn(arg);
    pager_set(current_bank);
}