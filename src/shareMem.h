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
#define SIZE 256

void * WR_shm();
void * RD_shm();
void munmapShm(void * data);
void unlinkShm();

#endif
