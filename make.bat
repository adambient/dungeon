REM compile source into binary
zcc +zx -vn -startup=31 -clib=sdcc_iy @zproject.lst -o main.bin -create-app
REM tidy up
del main.bin
del main_CODE.bin
del main_bss_user.bin