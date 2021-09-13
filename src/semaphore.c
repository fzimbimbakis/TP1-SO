// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "semaphore.h"      // ojota si se rompe por mismo nombre

sem_t * getSem_WR(char* name){
    sem_t * sem = sem_open(name, O_CREAT, O_RDWR,0);
    if(sem == SEM_FAILED){
        perror("sem_open WR");
        return SEM_FAILED;
    }
    return sem;
}

sem_t * getSem_RD(char* name){
    
    sem_t * sem = sem_open(name, O_CREAT);     // Podría llegar a pasar que lo inicie primero RD?
    
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


