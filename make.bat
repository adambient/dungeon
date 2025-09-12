REM create basic loader .tap
.\basic\bas2tap.exe .\basic\loader.bas -a loader.tap
REM compile source into binary
zcc +zx -vn -m -startup=5 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 @zproject.lst -o main.bin
REM zcc +zx -vn -m -startup=5 -clib=sdcc_iy @zproject.lst -o main.bin
REM create code .tap file
z88dk-appmake +zx -b main_CODE.bin --org 33536 --noloader --blockname code -o code.tap
REM create pasmo .tap file
.\pasmo\pasmo.exe --tap .\pasmo\main.asm pasmo.tap
REM cat .tap files to create loadable .tap
type loader.tap pasmo.tap code.tap > labyrinth.tap
REM tidy up
del loader.tap
del code.tap
del pasmo.tap
del *.bin