#include <semaphore.h>
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#define SEM_NAME "/semName"

sem_t * getSem_WR();
sem_t * getSem_RD();
void unlinkSem();

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#endif