// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <master.h>
int initPipesStreams(int slaves);
int closePipesStreams(int qSlaves);
void closeSomeStreams(int limit);
int runSlaves(int qSlaves);
void fileResultsExchange(int qSlaves, int cantFiles, sem_t *sem, char *data, const char **fileNames);
int sendInitialFiles(int slaves, char const *argv[]);
char *writeResults(char *data, FILE *resultsFile, sem_t *sem, int slaveToRead);

int pipeFiles[MAX_SLAVES][2];
int pipeResults[MAX_SLAVES][2];
FILE *streamFiles[MAX_SLAVES][2];
FILE *streamResults[MAX_SLAVES][2];
int slaves;

int main(int argc, char const *argv[])
{

    if (setvbuf(stdout, NULL, _IONBF, 0) != 0)
        return EXIT_FAILURE;

    if (argc <= 1)
    {
        perror("Debe ingresar los archivos por argumento");
        exit(5);
    }

    slaves = (argc >= 8) ? MAX_SLAVES : argc - 1;

    int cantFiles = argc - 1;

    // https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
    int pid = getpid();
    int length = snprintf(NULL, 0, "/%d", pid);
    char *name = malloc(length + 1);
    if (name == NULL)
        return EXIT_FAILURE;
    snprintf(name, length + 1, "/%d", pid);

    // SHM start
    char *data = WR_shm(name, cantFiles);
    if ((void *)-1 == data)
        return EXIT_FAILURE;

    void *firstDataPtr = data; // Para el munmap

    data += sizeof(int);

    if (initPipesStreams(slaves) == EXIT_FAILURE)
    {
        closePipesStreams(slaves);
        munmapShm(firstDataPtr, cantFiles);
        free(name);
        return EXIT_FAILURE;
    }

    if (runSlaves(slaves) == EXIT_FAILURE)
    {
        closePipesStreams(slaves);
        free(name);
        munmapShm(firstDataPtr, cantFiles);
        return EXIT_FAILURE;
    }

    // Semaphore start
    sem_t *sem = getSem_WR(name);
    if (sem == SEM_FAILED)
    {
        munmapShm(firstDataPtr, cantFiles);
        free(name);
        closePipesStreams(slaves);
        return EXIT_FAILURE;
    }

    // Name for view
    printf("%s\n", name);

    int sentFiles = sendInitialFiles(slaves, argv);

    fd_set currentFds, readyFds;
    FD_ZERO(&currentFds);

    int i;
    for (i = 0; i < slaves; i++)
    {
        FD_SET(pipeResults[i][0], &currentFds); //agego los fd que me importan
    }

    FILE *resultsFile = fopen("results.txt", "w");
    if (resultsFile == NULL)
    {
        free(name);
        munmapShm(firstDataPtr, cantFiles);
        sem_close(sem);
        closePipesStreams(slaves);
        perror("results.txt error");
        return EXIT_FAILURE;
    }

    int filesLeidos = 0;

    while (filesLeidos < cantFiles)
    {
        readyFds = currentFds;

        if (select(FD_SETSIZE, &readyFds, NULL, NULL, NULL) < 0)
        {
            perror("Error select");
            munmapShm(firstDataPtr, cantFiles);
            closePipesStreams(slaves);
            sem_close(sem);
            fclose(resultsFile);
            free(name);
            exit(1);
        }

        filesLeidos++;

        int i;
        for (i = 0; i < slaves; i++)
        {
            if (FD_ISSET(pipeResults[i][0], &readyFds))
            {

                data = writeResults(data, resultsFile, sem, i);
                if (sentFiles != cantFiles)
                {
                    fprintf(streamFiles[i][1], "%s", argv[++sentFiles]);
                    fprintf(streamFiles[i][1], "\n");
                }
                break;
            }
        }
    }

    if (closePipesStreams(slaves) == EXIT_FAILURE)
    {
        fclose(resultsFile);
        sem_close(sem);
        munmapShm(firstDataPtr, cantFiles);
        free(name);
        return EXIT_FAILURE;
    }

    fclose(resultsFile);

    // SHM finish
    munmapShm(firstDataPtr, cantFiles);

    // Semaphore finish
    sem_close(sem);
    free(name);
    return 0;
}

char *writeResults(char *data, FILE *resultsFile, sem_t *sem, int slaveToRead)
{
    size_t len = 0;
    char *string = NULL;
    getline(&string, &len, streamResults[slaveToRead][0]);
    sprintf(data, "%s", string);
    data += RESULT_SIZE;
    sem_post(sem);
    fprintf(resultsFile, "%s", string);
    free(string);
    return data;
}

int initPipesStreams(int slaves)
{
    int i;
    for (i = 0; i < slaves; i++)
    {
        pipe(pipeFiles[i]);
        pipe(pipeResults[i]);

        streamFiles[i][READ] = fdopen(pipeFiles[i][READ], "r");
        streamFiles[i][WRITE] = fdopen(pipeFiles[i][WRITE], "w");

        streamResults[i][READ] = fdopen(pipeResults[i][READ], "r");
        streamResults[i][WRITE] = fdopen(pipeResults[i][WRITE], "w");

        if (streamFiles[i][READ] == NULL)
        {
            closeSomeStreams(i);
            return EXIT_FAILURE;
        }
        if (streamFiles[i][WRITE] == NULL)
        {
            fclose(streamFiles[i][READ]);
            closeSomeStreams(i);
            return EXIT_FAILURE;
        }
        if (streamResults[i][WRITE] == NULL)
        {
            fclose(streamFiles[i][READ]);
            fclose(streamFiles[i][WRITE]);
            closeSomeStreams(i);
            return EXIT_FAILURE;
        }
        if (streamResults[i][READ] == NULL)
        {
            fclose(streamFiles[i][READ]);

            closeSomeStreams(i);
            fclose(streamFiles[i][WRITE]);

            fclose(streamResults[i][WRITE]);
            return EXIT_FAILURE;
        }

        if (setvbuf(streamFiles[i][WRITE], NULL, _IONBF, 0) != 0)
            return EXIT_FAILURE;
    }
    return 0;
}

void closeSomeStreams(int limit)
{
    int i;
    for (i = 0; i < limit; i++)
    {
        fclose(streamFiles[i][0]);
        fclose(streamFiles[i][1]);
        fclose(streamResults[i][0]);
        fclose(streamResults[i][1]);
    }
}

int closePipesStreams(int slaves)
{
    int i;
    for (i = 0; i < slaves; i++)
    {

        close(pipeFiles[i][0]);
        close(pipeFiles[i][1]);

        close(pipeResults[i][0]);
        close(pipeResults[i][1]);

        fclose(streamFiles[i][0]);
        fclose(streamFiles[i][1]);

        fclose(streamResults[i][0]);
        fclose(streamResults[i][1]);
    }
    return 0;
}

int runSlaves(int qSlaves)
{
    int i = 0;
    char *newargs = {NULL};
    while (i < qSlaves)
    {
        int pid;
        pid = fork();
        if (pid == 0)
        {
            int j = 0;
            while (j < qSlaves)
            {
                close(pipeFiles[j][1]);
                j++;
            }
            dup2(pipeFiles[i][0], STDIN);
            dup2(pipeResults[i][1], STDOUT);

            execv("./bin/slave", &newargs);
            perror("Fallo execv");
            return EXIT_FAILURE;
        }
        i++;
    }
    return 0;
}

int sendInitialFiles(int slaves, char const *argv[])
{
    int i;
    int sentFiles = 0;
    for (i = 0; i < slaves; i++)
    {
        fprintf(streamFiles[i][1], "%s", argv[++sentFiles]);
        fprintf(streamFiles[i][1], "\n");
    }
    return sentFiles;
}
