REM create basic loader .tap
.\basic\zmakebas -l -a @begin -o loader.build.tap .\basic\loader.bas

REM create pasmo .tap file
.\pasmo\pasmo.exe --tap .\pasmo\main.asm pasmo.build.tap

REM build o files using segments
zcc +zx -vn -m -startup=31 -clib=sdcc_iy bank3.c --codesegBANK_3 --constsegBANK_3 --datasegBANK_3 -c

REM build output binaries
zcc +zx -vn -m -startup=5 -clib=sdcc_iy @zproject.lst -o build

REM build .tap files to load using loader
z88dk-appmake +zx -b build_BANK_3.bin --org 49152 --noloader --blockname bank3 -o bank3.build.tap
z88dk-appmake +zx -b build_CODE.bin --org 32768 --noloader --blockname clang -o main.build.tap

REM create final .tap file
type loader.build.tap pasmo.build.tap main.build.tap bank3.build.tap > labyrinth.tap

REM tidy up
del *.bin
del *.o
del *.build.tap
del build