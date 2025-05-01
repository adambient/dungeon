REM compile source into binary
zcc +zx -vn -startup=5 -clib=sdcc_iy @zproject.lst -o main.bin -create-app
REM tidy up
del *.bin