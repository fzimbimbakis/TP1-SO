#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define SLAVES 5
#define STDIN 0
#define STDOUT 1
#define READ 0
#define WRITE 1


int main(int argc, char const *argv[])
{
//-------------------------------

/*
    int pipes[2];
    int pipe2[2];
    int len=0;
    FILE * streams[2];
    FILE * streams2[2];
    char eof=-1;
    pipe(pipes);
    pipe(pipe2);

    streams[0]=fdopen(pipes[0], "r");
    streams[1]=fdopen(pipes[1], "w");
    
    streams2[0]=fdopen(pipe2[0], "r");
    streams2[1]=fdopen(pipe2[1], "w");

    int pid=fork();

    if(pid==0){
        close(0);
        dup2( pipes[0], 0);
        dup2(pipe2[1], 1);
        execv("./bin/slave", NULL);
    }

    fprintf(streams[1],"Texto enviado desde master\n");
   // fprintf(streams[1], &eof), 
    close(streams[0]);
    fclose(streams[1]);
    waitpid(pid, NULL, 0 );
    char* string = NULL;
    getline(&string, &len, streams2[0]);
    printf(string);
    printf("Texto Master\n");
    
/*
//--------------------------------------------------------
//--------------------------------------------------------
        
    */
    
    if(argc <= 1){
        perror("Debe ingresar los archivos por argumento");
        exit(5);
    }

    int cant=argc;

    int mypipes[SLAVES*2][2];
    int contador=0;
    FILE* streams[SLAVES*2][2];

    while(contador < SLAVES*2){
        pipe(mypipes[contador]);
        streams[contador][READ] = fdopen(mypipes[contador][0], "r");
        streams[contador][WRITE] = fdopen(mypipes[contador][1], "w");
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
    while(contador < 2*SLAVES){
        pid=fork();
        if(pid==0){
            dup2(mypipes[contador][0],STDIN);//hijo lee en 00
            dup2(mypipes[contador+1][1],STDOUT);//hijo escribe en 11
            execv("./bin/slave",NULL);
            perror("Fallo execv");
        }
        contador+=2;
    }


    //-------------------------------------------------------------------//
    //esto no deberia estar, vamos a mandar los files en el while(i<cant)//
    //               con todos los fclose al final del main.c            //
    //-------------------------------------------------------------------//
    contador=0;
    while(contador < 2*SLAVES){
        fprintf(streams[contador][1],"./a.cnf\n");
        fclose(streams[contador][1]);
        contador+=2;
    }
  
    fd_set currentFds, readyFds;
    FD_ZERO (&currentFds);

    contador=0;
    while(contador<2*SLAVES){
        FD_SET (mypipes[contador+1][0], &currentFds);//agego todos los fd que
        contador+=2;                                //me interesa ver
    }

    int len=0;
    char* string = NULL;
    contador=0;

    while(contador<cant-1){//cant=cantidad de files para "minisatear"
        readyFds=currentFds;//select me destruye el set, por eso uso un auxiliar

        if(select(FD_SETSIZE, &readyFds, NULL, NULL, NULL) < 0){
            perror("Error select");
            exit(1);
        }
        contador++;//lei un archivo correctamente, uno menos para ver

        //si llego hasta aca, entonces el select detecto algo
        for(int i=0; i < 2*SLAVES; i++){    //pregunto por cada fd si esta en el set
            if(FD_ISSET(mypipes[i][0], &readyFds)){
                    getline(&string, &len, streams[i][0]);//le su buffer
                    printf("%d    %s", i, string);//imprimo salida
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