#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char* string = NULL;
    getline(&string, 0 , stdin);
    printf("%s", string);
    return 0;
}
