
#ifndef MASTER_H
#define MASTER_H

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <shareMem.h>
#include <ourSemaphore.h>

#define MAX_SLAVES 8
#define STDIN 0
#define STDOUT 1
#define READ 0
#define WRITE 1

#endif