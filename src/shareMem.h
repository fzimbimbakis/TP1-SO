/*  

Código tomado parcialmente del siguiente link.

https://linuxhint.com/posix-shared-memory-c-programming/

Gracias :)

*/
#ifndef SHARED_MEM_H
#define SHARE_MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/shmem"
#define RESULT_SIZE 256

void * WR_shm(int qResults);
void * RD_shm(int qResults);
void munmapShm(void * data, int qResults);
void unlinkShm();

#endif
