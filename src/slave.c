#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GREP_COMMAND "grep -o -e \".*Number of.*[0-9]\\+\" -e \".*CPU time.*\" -e \".*SATISFIABLE\" | sed 's/|  //' | tr '\n' '\t'"
#define COMMAND_SINTAXIS "minisat "

int main(int argc, char const *argv[])
{

     fclose(stderr);
    char *commandOutputLine = malloc(256);
    char *command = malloc(256);
    char *filePath;

    size_t len;
    ssize_t read;
    while (1) {
                filePath=NULL;
                len=0;
               // fclose(stdin);
                read = getline(&filePath, &len, stdin);
                
               // dprintf(1, " testt\n ");
               if(read==0 || read==-1){ // EOF o Error
               
                   free(filePath);
                   free(commandOutputLine);
                   free(command);
                   return read;
               }


                strcpy(command, COMMAND_SINTAXIS);
                command = strncat(command, filePath, 256);

                FILE *commandFilePipe = popen(command, "r");
                FILE *grepFilePipe = popen(GREP_COMMAND, "w");


               int i =0;
                while(fgets(commandOutputLine, 256, commandFilePipe)){
                    //printf("i= %d\n", i++);
                    fputs(commandOutputLine, grepFilePipe);
                }
                
                //fputs("Pid\n", grepFilePipe);
                // printf("%d",getpid());
                // printf("\t%s",filePath);

                pclose(commandFilePipe);
                pclose(grepFilePipe);
                dprintf(1, "Pid: %d\t%s", getpid(), filePath);
                //printf("%d", grepFilePipe);
            //     printf("\t%d",getpid());

            //    printf("\t%s\n",filePath);

                putchar('\n');
               //printf(salida);
    }
        return 0;
}
