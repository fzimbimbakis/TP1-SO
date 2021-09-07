
GCCFLAGS= -Wall -g -Wextra -Iinclude


all:
gcc $(GCCFLAGS) src/naster.c -o bin/master
gcc $(GCCFLAGS) src/slave.c -o bin/slave
gcc $(GCCFLAGS) src/view.c -o bin/view


clean:
rm -f bin/* 

.PHONY: all clean




