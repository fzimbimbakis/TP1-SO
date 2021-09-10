/*  

Código tomado parcialmente del siguiente link.

https://www.geeksforgeeks.org/posix-shared-memory-api/

*/
#include "shareMem.h"



void * open_WR_SHM(){

    int fd = shm_open(FILE_PATH, O_CREAT | O_RDWR, 0666);       // 666? !!!!!!!!!!!!!
    if(ERROR_CODE == fd)
        return (void *) ERROR_CODE;
    
    ftruncate(fd, BLOCK_SIZE);
    
    return mmap(NULL, BLOCK_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
}

void * open_RD_SHM(){

    int fd = shm_open(FILE_PATH, O_RDONLY, 0666);       // 666? !!!!!!!!!!!!!   Hace falta un O_CREAT?
    if(ERROR_CODE == fd)
        return (void *) ERROR_CODE;
    
    ftruncate(fd, BLOCK_SIZE);

    return mmap(NULL, BLOCK_SIZE, PROT_READ, MAP_SHARED, fd, 0);
}

void unlink_SHM(){
    shm_unlink(FILE_PATH);
}


