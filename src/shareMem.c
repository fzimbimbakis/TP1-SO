/*  

Código tomado parcialmente del siguiente link.

https://linuxhint.com/posix-shared-memory-c-programming/

Gracias :)

*/

#include "shareMem.h"

void * WR_shm(){
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);

    if(fd<0){
        perror("smh_open() WR");
        return EXIT_FAILURE;
    }

    ftruncate(fd, SIZE);

    int * data = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return data;
}

void * RD_shm(){
    int fd =shm_open(SHM_NAME, O_RDONLY, 066);
    
    if(fd<0){
        perror("smh_open() RD");
        return EXIT_FAILURE;
    }

    int * data= mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    return data;
}

void munmapShm(void * data){
    munmap(data, SIZE);
}

void unlinkShm(){
    shm_unlink(SHM_NAME);
}