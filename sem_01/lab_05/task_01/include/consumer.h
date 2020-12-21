#ifndef __CONSUMER_H__
#define __CONSUMER_H__

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "buffer.h"

#define BIN_SEM 0
#define BUFF_FULL 1
#define BUFF_EMPTY 2

#define ITER_CNT 8
#define SEM_SIZE 2

#define CONS_TIME_START 1
#define CONS_TIME_RANGE 4

#define BUFF_READ_ERROR 9
#define CONS_LOCK_ERROR 10
#define CONS_RELEASE_ERROR 11

int consumer_run(cbuffer_t *const buffer, const int sid, const int consid);

#endif