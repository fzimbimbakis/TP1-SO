#include "shareMem.h"
#include "semaphore.h"
#include <string.h>
int main(int argc, char const *argv[])
{   
    int qResults;
    if(argc > 1){
        qResults = atoi(argv[1]);
    } else{
        scanf("%d", &qResults);
    }
    
    // Share Mem.
    char * data = (char *) RD_shm(qResults);
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

        printf("%s", data);
        data += RESULT_SIZE;

    }

    unlinkSem(sem);
    
    munmapShm(data, qResults);
    unlinkShm();

    return 0;
}
