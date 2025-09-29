REM create basic loader .tap
.\basic\zmakebas -n loader -l -a @begin -o loader.build.tap .\basic\loader.bas

REM create pasmo .tap file
.\pasmo\pasmo.exe --name banker --tap .\pasmo\main.asm pasmo.build.tap

REM build o files using segments
zcc +zx -vn -m -startup=31 -clib=sdcc_iy .\bank3\bank3.c --codesegBANK_3 --constsegBANK_3 --datasegBANK_3 -c
REM TODO - use project list instead of filenames
zcc +zx -vn -m -startup=31 -clib=sdcc_iy .\sfx\tracker.asm .\sfx\clotho.asm .\sfx\hubbard.asm .\sfx\music.c .\sfx\beeps.c --codesegBANK_4 --constsegBANK_4 --datasegBANK_4 -c
zcc +zx -vn -m -startup=31 -clib=sdcc_iy .\gfx\gfx.asm --codesegBANK_0 --constsegBANK_0 --datasegBANK_0 -c

REM build output binaries
zcc +zx -vn -m -startup=5 -clib=sdcc_iy @zproject.lst -o build

REM build .tap files to load using loader
z88dk-appmake +zx -b build_BANK_3.bin --org 49152 --noloader --blockname bank3 -o bank3.build.tap
z88dk-appmake +zx -b build_BANK_4.bin --org 49152 --noloader --blockname sfx -o sfx.build.tap
z88dk-appmake +zx -b build_BANK_0.bin --org 49152 --noloader --blockname gfx -o gfx.build.tap
z88dk-appmake +zx -b build_CODE.bin --org 32768 --noloader --blockname core -o main.build.tap

REM create final .tap file
type loader.build.tap pasmo.build.tap main.build.tap bank3.build.tap sfx.build.tap gfx.build.tap > labyrinth.tap

REM tidy up
del *.bin
del /s /q "*.o"
del *.build.tap
del build