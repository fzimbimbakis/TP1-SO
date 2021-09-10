#include "semaphore.h"      // ojota si se rompe por mismo nombre

sem_t * getSem_WR(){
    sem_t * sem = sem_open(SEM_NAME, O_CREAT, O_WRONLY,1);
    // if(sem == SEM_FAILED)       // No se bien como manejar el error
    //     return SEM_FAILED;
    return sem;
}

sem_t * getSem_RD(){
    sem_t * sem = sem_open(SEM_NAME, O_CREAT);     // Podría llegar a pasar que lo inicie primero RD?
    // if(sem == SEM_FAILED)       // No se bien como manejar el error
    //     return SEM_FAILED;
    return sem;
}

void unlinkSem(){
    sem_unlink(SEM_NAME);
}


