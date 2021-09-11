/*  

Código tomado parcialmente del siguiente link.

https://linuxhint.com/posix-shared-memory-c-programming/

Gracias :)

*/

#include "shareMem.h"

void * WR_shm(int qResults){
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600); // Le saque el O_EXCL

    if(fd<0){
        perror("smh_open() WR");
        return EXIT_FAILURE;
    }

    ftruncate(fd, qResults * RESULT_SIZE);

    int * data = mmap(0, qResults * RESULT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return data;
}

void * RD_shm(int qResults){
    int fd =shm_open(SHM_NAME, O_RDONLY, 0666);
    
    if(fd<0){
        perror("smh_open() RD");
        return EXIT_FAILURE;
    }

    int * data= mmap(0, qResults * RESULT_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    return data;
}

void munmapShm(void * data, int qResults){
    munmap(data, qResults * RESULT_SIZE);
}

void unlinkShm(){
    shm_unlink(SHM_NAME);
}