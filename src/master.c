#include "master.h"


int main(int argc, char const *argv[])
{
    if(argc <= 1){
        perror("Debe ingresar los archivos por argumento");
        exit(5);
    }

    int cantFiles=argc-1;
    int contador=0;

    int pipeFiles[SLAVES][2];
    int pipeResults[SLAVES][2];
    FILE* streamFiles[SLAVES][2];
    FILE* streamResults[SLAVES][2];


    while(contador < SLAVES){
        pipe(pipeFiles[contador]);
        pipe(pipeResults[contador]);

        streamFiles[contador][READ] = fdopen(pipeFiles[contador][0], "r");
        streamFiles[contador][WRITE] = fdopen(pipeFiles[contador][1], "w");

        streamResults[contador][READ] = fdopen(pipeResults[contador][0], "r");
        streamResults[contador][WRITE] = fdopen(pipeResults[contador][1], "w");

        contador++;
    }
    
    /*
        pipe1   [r] [w]     master WRITE en 1, slave READ 0, fprintf(stream[0][1]) --> slave lee getline()
        pipe2   [r] [w]     slave WRITE en 1,  master WRITE 1, slave escribe printf()--> master lee por stream[1][0]
    */

    contador=0;
    int pid;
    while(contador < SLAVES){
        pid=fork();
        if(pid==0){
            dup2(pipeFiles[contador][0],STDIN);//hijo lee en 00
            dup2(pipeResults[contador][1],STDOUT);//hijo escribe en 11
            execv("./bin/slave",NULL);
            perror("Fallo execv");
        }
        contador++;
    }


    contador=0;
    int sentFiles=0;

    while(contador < SLAVES && contador < cantFiles){
        fprintf(streamFiles[contador][1],argv[++sentFiles]);
        fprintf(streamFiles[contador][1],"\n");

        fflush(streamFiles[contador][1]);
        contador++;
    }
  
    fd_set currentFds, readyFds;
    FD_ZERO (&currentFds);

    contador=0;
    while(contador<SLAVES){
        FD_SET (pipeResults[contador][0], &currentFds);//agego todos los fd que
        contador++;                                //me interesa ver
    }

    int len=0;
    char* string =NULL;
    contador=0;

    // SHM start
    char * data = WR_shm(cantFiles);
    printf("%d\n", cantFiles);

    // Semaphore start
    sem_t * sem = getSem_WR();

    printf("8 segundos para llamar a view.\n");
    sleep(8);

    while(contador<cantFiles){//cant=cantidad de files para "minisatear"
        readyFds=currentFds;//select me destruye el set, por eso uso un auxiliar

        //printf("Select %d\n",contador);
        
        if(select(FD_SETSIZE, &readyFds, NULL, NULL, NULL) < 0){
            perror("Error select");
            exit(1);
        }
        
        contador++;//lei un archivo correctamente, uno menos para ver

        //si llego hasta aca, entonces el select detecto algo
        int i;
        for(i=0; i < SLAVES; i++){    //pregunto por cada fd si esta en el set
            if(FD_ISSET(pipeResults[i][0], &readyFds)){
                
                    getline(&string, &len, streamResults[i][0]);
                    sprintf(data, "%s", string);
                    data += RESULT_SIZE;
                    sem_post(sem);
                    if(sentFiles!=cantFiles){
                        fprintf(streamFiles[i][1],argv[++sentFiles]);
                        fprintf(streamFiles[i][1],"\n");
                        fflush(streamFiles[i][1]);
                    }
                    break;//salteo las otras comparaciones, ya lei el que quiero
            }
        }
      

    }

    contador=0;
    while(contador < SLAVES){

        close(pipeFiles[contador][0]);
        close(pipeFiles[contador][1]);

        close(pipeResults[contador][0]);
        close(pipeResults[contador][1]);
        
        
        
        
        fclose(streamFiles[contador][0]); //lee slave
        fclose(streamFiles[contador][1]); //escribe master

        fclose(streamResults[contador][0]);  //lee master
        fclose(streamResults[contador][1]); //escribe slave

        contador++;
    }

    // SHM finish
    munmapShm(data, cantFiles);
    unlinkShm();

    // Semaphore finish
    unlinkSem(sem);

    return 0;
}