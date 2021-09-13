GCC_FLAGS= -Wall -g -Wextra -I ./include
SHMEM_SEM_FLAGS=-lrt -pthread
all: master slave view

master: src/master.c
	gcc $(GCC_FLAGS) $(SHMEM_SEM_FLAGS) src/semaphore.c src/shareMem.c src/master.c -o bin/master

slave: src/slave.c
	gcc $(GCC_FLAGS) src/slave.c -o bin/slave

view: src/view.c
	gcc $(GCC_FLAGS) $(SHMEM_SEM_FLAGS) src/semaphore.c src/shareMem.c src/view.c -o bin/view


clean:
	rm -f bin/*

.PHONY: all clean




