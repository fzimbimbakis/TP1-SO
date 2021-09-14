
#ifndef SEMAPHORE_H
#define SEMAPHORE_H


#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
sem_t * getSem_WR(char* name);
sem_t * getSem_RD(char* name);
void unlinkSem(sem_t * sem, char * name);



#endif