/*  

Código tomado parcialmente del siguiente link.

https://linuxhint.com/posix-shared-memory-c-programming/

Gracias :)

*/

#include "shareMem.h"
//#include <errno.h>

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
    errno=0;
    int fd =shm_open(SHM_NAME, O_CREAT | O_RDONLY, 0666);
    /*switch(errno){
        case ENOENT: printf("ENOENT"); break;
        case AECCES: printf("ACCES"); break;
        case EEXIST: printf("EXIST"); break;
        case EINVAL: printf("INVAL"); break;
        case EMFILE: printf("FILE"); break;
        case ENAMETOOLONG: printf("NAMETOO"); break;
        case ENFILE: printf("ENFILE"); break;
    }*/
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