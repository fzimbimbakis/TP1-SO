// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "master.h"
void initPipesStreams();
void closePipesStreams(int qSlaves);
void runSlaves(int qSlaves);
void fileResultsExchange(int qSlaves, int cantFiles, sem_t * sem, char * data, const char ** fileNames);

    int pipeFiles[SLAVES][2];
    int pipeResults[SLAVES][2];
    FILE* streamFiles[SLAVES][2];
    FILE* streamResults[SLAVES][2];

int main(int argc, char const *argv[])
{

    if(argc <= 1){
        perror("Debe ingresar los archivos por argumento");
        exit(5);
    }
    int cantFiles=argc-1;
    // SHM start
    // https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
    int pid = getpid();
    int length = snprintf( NULL, 0, "/%d", pid );
    char* name = malloc( length + 1 );
    if(name==NULL){
        perror("malloc error");
        return EXIT_FAILURE;
    }
    snprintf( name, length + 1, "/%d",  pid);

    char * data = WR_shm(name, cantFiles);
        if((void *) -1 == data)
        return EXIT_FAILURE;

    void * aux = data;
    data += sizeof(int);
    FILE* resultsFd=fopen("results.txt","w");
    if(resultsFd==NULL){
        perror("results.txt error");
        return EXIT_FAILURE;
    }

    
    
    initPipesStreams();
    

    runSlaves(SLAVES);          // El tema de generar un stack y despues un fork cambia algo?
    

    // Semaphore start
    sem_t * sem = getSem_WR(name);
    if(sem==SEM_FAILED){
        closePipesStreams(SLAVES);
        fclose(resultsFd);
        return EXIT_FAILURE;
    }
    
    printf("%s\n", name);

    //fileResultsExchange(SLAVES, cantFiles, sem, data, argv);
    int contador=0;
    int sentFiles=0;

    while(contador < SLAVES && contador < cantFiles){
        fprintf(streamFiles[contador][1],"%s", argv[++sentFiles]);
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

    size_t len=0;
    char* string =NULL;
    contador=0;


   // printf("8 segundos para llamar a view.\n");
    //sleep(8);

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
                    fprintf(resultsFd, "%s", string);
                    if(sentFiles!=cantFiles){
                        fprintf(streamFiles[i][1], "%s", argv[++sentFiles]);
                        fprintf(streamFiles[i][1],"\n");
                        fflush(streamFiles[i][1]);
                    }
                    break;//salteo las otras comparaciones, ya lei el que quiero
            }
        }
    }

    free(string);

    //sleep(5);

    closePipesStreams(SLAVES);

    fclose(resultsFd);


    // SHM finish
    munmapShm(aux, cantFiles);
    free(name);
    //unlinkShm();

    // Semaphore finish
    sem_close(sem);
    //unlinkSem(sem);
   // perror("master\n");

    return 0;
}












void initPipesStreams(){
    int contador=0;
    while(contador < SLAVES){
        pipe(pipeFiles[contador]);
        pipe(pipeResults[contador]);

        streamFiles[contador][READ] = fdopen(pipeFiles[contador][0], "r");
        streamFiles[contador][WRITE] = fdopen(pipeFiles[contador][1], "w");

        streamResults[contador][READ] = fdopen(pipeResults[contador][0], "r");
        streamResults[contador][WRITE] = fdopen(pipeResults[contador][1], "w");

        contador++;
    }
}

void closePipesStreams(int qSlaves){
    int contador=0;
    while(contador < qSlaves){

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
}

void runSlaves(int qSlaves){
    int contador=0;
    char * newargs = {NULL};
    while(contador < qSlaves){
        int pid;
        pid=fork();
        if(pid==0){
            int j=0;
            while(j<qSlaves){
                close(pipeFiles[j][1]);
                //fclose(streamFiles[j][1]);
                //fclose(streamResults[j][1]);
                j++;
            }
            dup2(pipeFiles[contador][0],STDIN);//hijo lee en 00
            dup2(pipeResults[contador][1],STDOUT);//hijo escribe en 11

            execv("./bin/slave",&newargs);
            perror("Fallo execv");
        }
        contador++;
    }
}

// void writeResults(char * data, sem_t * sem, FILE * file);

// void fileResultsExchange(int qSlaves, int cantFiles, sem_t * sem, char * data, const char ** fileNames){
//     int contador=0;
//     int sentFiles=0;

//     while(contador < qSlaves && contador < cantFiles){
//         fprintf(streamFiles[contador][1],fileNames[++sentFiles]);
//         fprintf(streamFiles[contador][1],"\n");

//         fflush(streamFiles[contador][1]);
//         contador++;
//     }
  
//     fd_set currentFds, readyFds;
//     FD_ZERO (&currentFds);

//     contador=0;
//     while(contador<qSlaves){
//         FD_SET (pipeResults[contador][0], &currentFds);//agego todos los fd que
//         contador++;                                //me interesa ver
//     }

//     // size_t len=0;
//     // char* string =NULL;
//     contador=0;

//     sleep(8); // Tiempo para llamar a view

//     while(contador<cantFiles){
        
//         readyFds=currentFds;    //select me destruye el set, por eso uso un auxiliar
        
//         if(select(FD_SETSIZE, &readyFds, NULL, NULL, NULL) < 0){
//             perror("Error select");
//             exit(1);
//         }

//         // Select (:
//         contador++;

//         int i;
//         for(i=0; i < qSlaves; i++){    //pregunto por cada fd si esta en el set
            
//             if(FD_ISSET(pipeResults[i][0], &readyFds)){

//                     writeResults(data, sem, streamResults[i][0]);
                    
//                     if(sentFiles!=cantFiles){
//                         fprintf(streamFiles[i][1],fileNames[++sentFiles]);
//                         fprintf(streamFiles[i][1],"\n");
//                         fflush(streamFiles[i][1]);
//                     }

//                     break;  //Ya leyó
//             }

//         }
      

//     }
// }

// void writeResults(char * data, sem_t * sem, FILE * file){
//     char* string =NULL;
//     size_t len=0;
//     getline(&string, &len, file);
//     sprintf(data, "%s", string);
//     data += RESULT_SIZE;
//     sem_post(sem);
// }