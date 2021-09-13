#include "shareMem.h"
#include "semaphore.h"
#include <string.h>
int main(int argc, char *argv[])
{   
    char * name;
    if(argc > 1){
        name = argv[1];
    } else{
        name = malloc(256);
        scanf("%s", name);
    }
    
    // Share Mem.
    int qResults;
    char * data = (char *) RD_shm(name, &qResults);
    void * aux = (void *) data;
    data += sizeof(int);

    if(data==(void *)-1) 
        return EXIT_FAILURE;

    // Semaphore
    sem_t * sem = getSem_RD();
    if(sem==SEM_FAILED) 
        return EXIT_FAILURE;
    
    int i;
    for (i = 0; i < qResults; i++)
    {
        sem_wait(sem);

        printf(data);
        data += RESULT_SIZE;

    }

    //unlinkSem(sem);
    
    munmapShm(aux, qResults);
    unlinkShm();

    return 0;
}
