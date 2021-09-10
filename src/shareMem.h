/*  

Código tomado parcialmente del siguiente link.

https://www.geeksforgeeks.org/posix-shared-memory-api/

*/
#ifndef SHARED_MEM_H
#define SHARE_MEM_H

#define BLOCK_SIZE 4096
#define FILE_PATH "./results.txt"
#define ERROR_CODE -1

void * open_WR_SHM();
void * open_RD_SHM();
void unlink_SHM();

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdio.h>

#endif
