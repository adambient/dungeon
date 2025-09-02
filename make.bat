REM create basic loader .tap
.\utils\bas2tap.exe loader.bas -a
REM compile source into binary
zcc +zx -vn -startup=5 -clib=sdcc_iy @zproject.lst -o main.bin
REM create code .tap file
z88dk-appmake +zx -b main_CODE.bin --org 24500 --noloader --blockname code -o code.tap
REM cat .tap files to create loadable .tap
type loader.tap code.tap > labyrinth.tap
REM tidy up
del loader.tap
del code.tap
del *.bin