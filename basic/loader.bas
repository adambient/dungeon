
@begin:
CLEAR 32511
# load pasmo code
LOAD "" CODE 32512
# load bank switch code
DEF FN M(b)=USR 32512
# load clang code
LOAD "" CODE 32768
# switch to bank 3
RANDOMIZE FN M(3)
# load bank 3
LOAD "" CODE 49152
# load bank 4 (sfx)
RANDOMIZE FN M(4)
LOAD "" CODE 49152
# load bank 0 (gfx)
RANDOMIZE FN M(0)
LOAD "" CODE 49152
# call clang start
@mainloop:
CLS
# TODO - welcome screen
PRINT "The Dark & Mysterious Labyrinth"
PRINT "Press any key..."
PAUSE 0
RANDOMIZE USR 32768
GOTO @mainloop