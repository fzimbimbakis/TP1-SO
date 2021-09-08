GCCFLAGS= -Wall -g -Wextra -Iinclude

master: src/master.c
	gcc $(GCCFLAGS) src/master.c -o bin/master

slave: src/slave.c
	gcc $(GCCFLAGS) src/slave.c -o bin/slave

view: src/view.c
	gcc $(GCCFLAGS) src/view.c -o bin/view



all: master slave view

clean:
	rm -f bin/*

.PHONY: all clean




