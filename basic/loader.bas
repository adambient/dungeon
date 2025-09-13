
@begin:
CLEAR 24499
# load pasmo code
LOAD "" CODE 24500
# load bank switch code
DEF FN M(b)=USR 24550
# load clang code
LOAD "" CODE 33536
# switch to bank 3
RANDOMIZE FN M(3)
PRINT 1
# load bank 3
LOAD "" CODE 49152
PRINT 2
# switch back to bank 1
RANDOMIZE FN M(1)
PRINT 3
# call interrupt init
RANDOMIZE USR 24500
PRINT 4
# call clang start
CLS
RANDOMIZE USR 33536
PRINT 5