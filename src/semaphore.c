#include "semaphore.h"      // ojota si se rompe por mismo nombre

sem_t * getSem_WR(){
    sem_t * sem = sem_open(SEM_NAME, O_CREAT, O_WRONLY,0);
    return sem;
}

sem_t * getSem_RD(){
    sem_t * sem = sem_open(SEM_NAME, O_CREAT);     // Podría llegar a pasar que lo inicie primero RD?
    return sem;
}

void unlinkSem(){
    sem_unlink(SEM_NAME);
}


