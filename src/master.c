#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define SLAVES 2
#define STDIN 0
#define STDOUT 1
#define READ 0
#define WRITE 1


int main(int argc, char const *argv[])
{
    
    if(argc <= 1){
        perror("Debe ingresar los archivos por argumento");
        exit(5);
    }

    int cantFiles=argc-1;

    int pipeFiles[SLAVES][2];
    int pipeResults[SLAVES][2];
    int contador=0;
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

        pipe3   [r] [w]
        pipe4   [r] [w]

        pipe5   [r] [w]
        pipe6   [r] [w]

        pipe7   [r] [w]
        pipe8   [r] [w]

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


    //-------------------------------------------------------------------//
    //esto no deberia estar, vamos a mandar los files en el while(i<cant)//
    //               con todos los fclose al final del main.c            //
    //-------------------------------------------------------------------//
    contador=0;
    int sentFiles=0;
    int eof=-1;
    while(contador < SLAVES && contador < cantFiles){
        fprintf(streamFiles[contador][1],argv[++sentFiles]);
        fprintf(streamFiles[contador][1],"\n");
        //printf("antes de EOF\n");
        //fprintf(streamFiles[contador][1],"\0");
        //printf("despues de EOF\n");
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
    char* string = NULL;
    contador=0;

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

                    //fgets(string, 1024, streamResults[i][0]);
                    getline(&string, &len, streamResults[i][0]);//lee su buffer
                    printf("%d    %s", i, string);//imprimo salida
/*--------------------------------------------------------------------------------------
        Hay que ver como pasarle otro archivo al esclavo que ya termino de procesar*/

                    if(sentFiles!=cantFiles){
                        //printf("asdfjhsajdkfh\n");
                        //streamFiles[i][1] = fdopen(pipeFiles[i][1], "w");
                        //printf("open = %d\n",streamFiles[i][1]);
                        fprintf(streamFiles[i][1],argv[++sentFiles]);
                       // printf("sssasdaasdf\n");
                        fprintf(streamFiles[i][1],"\n");
                        //fprintf(streamFiles[i][1], "\0");
                        fflush(streamFiles[i][1]);

                    }

/*---------------------------------------------------------------------------------------*/
                    break;//salteo las otras comparaciones, ya lei el que quiero
            }
        }
        /*
        for(int i=0; i < FD_SETSIZE; i++){
            if(FD_ISSET(i, &readyFds)){
                if (i==mypipes[1][0]){
                    getline(&string, &len, streams[1][0]);
                    printf(string);
                }
                else if (i==mypipes[3][0]) {
                    getline(&string, &len, streams[3][0]);
                    printf(string);}
                else if (i==mypipes[5][0]) {
                    getline(&string, &len, streams[5][0]);
                    printf(string);}
                else if (i==mypipes[7][0]) {
                    getline(&string, &len, streams[7][0]);
                    printf(string);}
                else if (i==mypipes[9][0]) {
                    getline(&string, &len, streams[9][0]);
                    printf(string);}
                else printf("%d\n", i);
            }
        }
*/

/*
        for (int i = 0; i < 2*SLAVES; i+=2){
            if(FD_ISSET(mypipes[i][0], &readyFds)){
                getline(&string, &len, streams[i][0]);
                printf(string);
            }
        }
*/

    }

    return 0;
}


/*
h1-f1--------------
h2-f2------
h3-f3-   f4--------


*/