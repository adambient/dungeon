
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
# load bank 6 (sfx)
RANDOMIZE FN M(6)
LOAD "" CODE 49152
# load bank 7 (gfx)
RANDOMIZE FN M(7)
LOAD "" CODE 49152
# call clang start
CLS
RANDOMIZE USR 32768