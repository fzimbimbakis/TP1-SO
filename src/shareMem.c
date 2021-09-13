// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*  

Código tomado parcialmente del siguiente link.

https://linuxhint.com/posix-shared-memory-c-programming/

Gracias :)

*/

#include "shareMem.h"
//#include <errno.h>

void * WR_shm(char * name, int qResults){

    int fd = shm_open(name, O_CREAT | O_RDWR, 0600); // Le saque el O_EXCL
    if(fd<0){
        perror("smh_open() WR");
        return (void *)-1;
    }
    
    ftruncate(fd, sizeof(int) + qResults * RESULT_SIZE);

    int * data = mmap(0, sizeof(int) + qResults * RESULT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(data==MAP_FAILED)
        {
            perror("mmap WR");
            return MAP_FAILED;
        }

    *data = qResults;

    close(fd);
    return data;
}

void * RD_shm(char * name, int * qResults){
    
    int fd =shm_open(name, O_CREAT | O_RDONLY, 0666);
    if(fd<0){
        perror("smh_open() RD");
        return (void *) -1;
    }

    int * intMap = (int *)mmap(0, sizeof(int), PROT_READ, MAP_SHARED, fd, 0);
    * qResults = *intMap;
    munmapShm((void *)intMap, sizeof(int));

    void * data = mmap(0, sizeof(int) + (*qResults) * RESULT_SIZE, PROT_READ, MAP_SHARED, fd, 0);

    close(fd);
    return data;
}

void munmapShm(void * data, int qResults){
    munmap(data, sizeof(int) + qResults * RESULT_SIZE);
}

void unlinkShm(char * name){
    shm_unlink(name);
}