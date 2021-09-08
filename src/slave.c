#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char const *argv[])
{
     fclose(stderr);
     
            char *minisatLine = malloc(256);
           char *command = malloc(256);
           
           char *filePath;
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

                FILE *minisatFilePipe = popen(command, "r");
                FILE *grepFilePipe = popen("grep -o \"UN.*\"", "w");
                
                
                while(fscanf(minisatFilePipe, "%s\n", minisatLine)!=EOF){
                    fprintf(grepFilePipe, "%s\n", minisatLine);
                }

                fclose(minisatFilePipe);
                fclose(grepFilePipe);
    }     
        return 0;
}
