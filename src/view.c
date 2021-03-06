// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shareMem.h>
#include <ourSemaphore.h>
#include <string.h>
int main(int argc, char *argv[])
{
    char *name;
    if (argc > 1)
    {
        name = argv[1];
    }
    else
    {
        int i = 0;
        name = malloc(256);
        if (name == NULL)
            return EXIT_FAILURE;

        char c;
        while ((c = getchar()) != '\n' && i < 256)
        {
            name[i] = c;
            i++;
        }
    }

    // Share Mem.
    int qResults;
    char *results = (char *)RD_shm(name, &qResults);
    void *aux = (void *)results;
    results += sizeof(int);

    if (results == (void *)-1)
        return EXIT_FAILURE;

    // Semaphore
    sem_t *sem = getSem_RD(name);
    if (sem == SEM_FAILED)
        return EXIT_FAILURE;

    int i;
    for (i = 0; i < qResults; i++)
    {
        sem_wait(sem);

        printf("%s", results);
        results += RESULT_SIZE;
    }

    munmapShm(aux, qResults);
    unlinkShm(name);
    unlinkSem(sem, name);

    return 0;
}
