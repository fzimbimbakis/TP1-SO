#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char const *argv[])
{
     fclose(stderr);
            FILE *minisatPipe;
            FILE *grepPipe;
           char *filePath;
            char *line = malloc(256);
           char *command = malloc(256);
           strcpy(command, "minisat ");
           size_t len;
           ssize_t read;
    
    while (1) {
                filePath=NULL;
                len=0;
               read = getline(&filePath, &len, stdin);
    
               if(read==0 || read==-1){
                   free(filePath);
                   return read;
               }
                strcpy(command, "minisat ");
                command = strncat(command, filePath, 256);
                //printf("%s", command);
                FILE *minisatFilePipe = popen(command, "r");
                FILE *grepFilePipe = popen("grep -o \"UN.*\"", "w");
                
                
                
                while(fscanf(minisatFilePipe, "%s\n", line)!=EOF){
                    fprintf(grepFilePipe, "%s\n", line);
                }
                fclose(minisatFilePipe);
                fclose(grepFilePipe);
    }     
           // free(filePath);
        return 0;
}