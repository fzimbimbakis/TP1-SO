#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GREP_COMMAND "grep -o -e \".*Number of.*[0-9]\\+\" -e \".*CPU time.*\" -e \".*SATISFIABLE\""
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
               read = getline(&filePath, &len, stdin);
    
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
                
                

                while(fgets(commandOutputLine, 256, commandFilePipe)){
                    fputs(commandOutputLine, grepFilePipe);
                }

                fclose(commandFilePipe);
                fclose(grepFilePipe);
    }     
        return 0;
}
