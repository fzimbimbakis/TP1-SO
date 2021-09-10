#include <stdio.h>
#include <stdlib.h>

main(){
    int pipeGrep[2];
    pipe(pipeGrep);
    dup2(pipeGrep[1],1);
    FILE* stream[2];
    stream[0] = fdopen(pipeGrep[0], "r");
    stream[1] = fdopen(pipeGrep[1], "w");

    printf("esto no deberia verse");

    dup2(1, pipeGrep[1]);

    dprintf(stdin, "Esto si");

}