// imported from banker.asm
extern unsigned int banker_get(void) __z88dk_fastcall;
extern void banker_set(unsigned char bank) __z88dk_fastcall;

void exec_far(void (*fn)(void), unsigned char far_bank)
{
    unsigned char current_bank = banker_get();
    banker_set(far_bank);
    fn();
    banker_set(current_bank);
}