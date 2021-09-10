#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GREP_COMMAND "grep -o -e \".*Number of.*[0-9]\\+\" -e \".*CPU time.*\" -e \".*SATISFIABLE\" | sed 's/|  Number of variables:/ /' | sed 's/[^[:print:]]//g'"
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
                char* salida=malloc(1024);
                filePath=NULL;
                len=0;
               read = getline(&filePath, &len, stdin);
    
               if(read==0 || read==-1){ // EOF o Error
                   free(filePath);
                   free(commandOutputLine);
                   free(command); 
                   return read;
               }
               
               snprintf(salida,1024,"%d\t%s",getpid(),filePath);
                printf(salida);

                strcpy(command, COMMAND_SINTAXIS);
                command = strncat(command, filePath, 256);

                FILE *commandFilePipe = popen(command, "r");
                FILE *grepFilePipe = popen(GREP_COMMAND, "w");
                
                
                /*int pipeGrep[2];
                pipe(pipeGrep);
                dup2(pipeGrep[1],1);
                FILE* stream[2];
                stream[0] = fdopen(pipeGrep[0], "r");
                stream[1] = fdopen(pipeGrep[1], "w");*/

                while(fgets(commandOutputLine, 256, commandFilePipe)){
                    fputs(commandOutputLine, grepFilePipe);
                }


              
                int i =0;
                
                //fprintf(stdin, )

                fclose(commandFilePipe);
                fclose(grepFilePipe);
    }     
        return 0;
}
