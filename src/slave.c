// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GREP_COMMAND "grep -o -e \".*Number of.*[0-9]\\+\" -e \".*CPU time.*\" -e \".*SATISFIABLE\" | sed 's/|  //' | tr '\n' '\t'"
#define COMMAND_SINTAXIS "minisat "

int main()
{

 
    char commandOutputLine[256];
    char command[256];
    char *filePath;

    size_t len;
    while (1) {
                ssize_t read;
                filePath=NULL;
                len=0;
            
                read = getline(&filePath, &len, stdin);
                if(read==0 || read==-1){ 
                   free(filePath);
                   return read;
                }


                strcpy(command, COMMAND_SINTAXIS);
                strcat(command, filePath);

                FILE *commandFilePipe = popen(command, "r");
                FILE *grepFilePipe = popen(GREP_COMMAND, "w");


               
                while(fgets(commandOutputLine, 256, commandFilePipe)){
                    fputs(commandOutputLine, grepFilePipe);
                }

                pclose(commandFilePipe);
                pclose(grepFilePipe);
                dprintf(1, "Pid: %d\t%s", getpid(), filePath);

                
    }
        return 0;
}