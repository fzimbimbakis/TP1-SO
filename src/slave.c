#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char* string = NULL;
    int n=0;
    printf("Texto Slave\n");
    char c;
    getline(&string, &n , stdin);
    printf(string);
    return 0;
}
