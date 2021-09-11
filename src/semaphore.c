#include "semaphore.h"      // ojota si se rompe por mismo nombre

sem_t * getSem_WR(){
    sem_t * sem = sem_open(SEM_NAME, O_CREAT, O_RDWR,0);
    if(sem == SEM_FAILED){
        perror("sem_open WR");
        return SEM_FAILED;
    }
    return sem;
}

sem_t * getSem_RD(){
    
    sem_t * sem = sem_open(SEM_NAME, O_CREAT);     // Podría llegar a pasar que lo inicie primero RD?
    
    if(sem == SEM_FAILED){
        perror("sem_open RD");
        return SEM_FAILED;
    }
    return sem;
}

void unlinkSem(sem_t * sem){
    sem_close(sem);
    sem_unlink(SEM_NAME);
}


