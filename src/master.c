#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
 #include <sys/types.h>

#define SLAVES 5
#define STDIN 0
#define STDOUT 1
#define READ 0
#define WRITE 1


int main(int argc, char const *argv[])
{

    
    int pipes[2];
    FILE * streams[2];

    pipe(pipes);

    streams[0]=fdopen(pipes[0], "r");
    streams[0]=fdopen(pipes[1], "w");
    

    int pid=fork();
    if(pid==0){
        dup2(0, pipes[0]);
        dup2(1, pipes[1]);
        execv("./slave",NULL);
    }
    fprintf(streams[0],"Hola");
        
  /*  if(argc <= 1){
        perror("Debe ingresar los archivos por argumento");
    }
    int mypipes[SLAVES*2][2];
    int contador=0;
    FILE* streams[SLAVES*2][2];

    while(contador < SLAVES*2){
        pipe(mypipes[contador]);
        streams[contador][READ] = fdopen(mypipes[contador][0], "r");
        streams[contador][WRITE] = fdopen(mypipes[contador][1], "w");
        contador++;
    }*/
    
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
    /*
    contador=0;
    int pid;
    while(contador < SLAVES){
        pid=fork();
        if(pid==0){
            dup2(STDOUT, mypipes[contador+1][1]);//hijo escribe en 11
            dup2(STDIN, mypipes[contador][0]);//hijo lee en 00
            execv("./slave");
            perror("Fallo execv");
        }    

    }

    while()
*/
    return 0;
}
