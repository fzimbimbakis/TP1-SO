#include "shareMem.h"
#include "semaphore.h"

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
    char * actualResult = malloc(RESULT_SIZE);

    // Semaphore
    sem_t * sem = getSem_RD;
    
    int i;
    for (i = 0; i < qResults; i++)
    {
        
        wait(sem);

        sscanf(data, "%s\n", actualResult);
        printf("%s\n", actualResult);
        data += RESULT_SIZE;

    }
    
    return 0;
}
