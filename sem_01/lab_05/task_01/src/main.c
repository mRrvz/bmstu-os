#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h> 
#include <unistd.h>
#include <wait.h>

#include "buffer.h"
#include "consumer.h"
#include "producer.h"

#define PROD_CNT 3
#define CONS_CNT 3

#define PERMISSIONS S_IRWXU | S_IRWXG | S_IRWXO
#define SHMAT_ERR_RET (void *)-1

#define SEM_CNT 3
#define BIN_SEM 0
#define BUFF_FULL 1
#define BUFF_EMPTY 2

#define FREE 1

#define SHMGET_ERROR 1
#define SHMAT_ERROR 2
#define FORK_ERROR 3
#define WAIT_ERROR 4
#define SHUTDOWN_ERROR 5
#define SEMGET_ERROR 12

int main() {
    setbuf(stdout, NULL);

    int fd = shmget(IPC_PRIVATE, sizeof(cbuffer_t), PERMISSIONS | IPC_CREAT);
    if (-1 == fd) {
        perror("Error while creating shared memory.\n");
        return SHMGET_ERROR;
    }

    cbuffer_t *buffer = shmat(fd, 0, 0);
    if (SHMAT_ERR_RET == buffer) {
        perror("Error while creating shmat.\n");
        return SHMAT_ERROR;
    }

    if (BUF_ERR == init_buffer(buffer)) {
        return BUF_ERR;
    }

    int sid = semget(IPC_PRIVATE, SEM_CNT, PERMISSIONS | IPC_CREAT);
    if (-1 == sid) {
        perror("Error while creating array of semaphores.\n");
        return SEMGET_ERROR;
    }

    semctl(sid, BIN_SEM, SETVAL, FREE);
    semctl(sid, BUFF_EMPTY, SETVAL, N);
    semctl(sid, BUFF_FULL, SETVAL, 0);

    for (size_t i = 0; i < PROD_CNT; i++) {
        int child_pid = fork();

        if (-1 == child_pid) {
            perror("Error while fork (producer).");
            return FORK_ERROR;
        } else if (0 == child_pid) {
            producer_run(buffer, sid, i);
            return OK;
        }
    }

    for (size_t i = 0; i < CONS_CNT; i++) {
        int child_pid = fork();

        if (-1 == child_pid) {
            perror("Error while fork (consumer).");
            return FORK_ERROR;
        } else if (0 == child_pid) {
            consumer_run(buffer, sid, i);
            return OK;
        }
    }

    for (size_t i = 0; i < PROD_CNT + CONS_CNT; i++) {
        int statval;
        
        if (-1 == wait(&statval)) {
            perror("Error with child process.\n");
            return WAIT_ERROR;
        }

        if (!WIFEXITED(statval)) {
            fprintf(stderr, "Children process %lu terminated abnormally.", i);
        }
    }

    if (-1 == shmdt((void *)buffer) || -1 == shmctl(fd, IPC_RMID, NULL) || -1 == semctl(sid, IPC_RMID, 0)) {
        perror("Error while shutdown.\n");
        return SHUTDOWN_ERROR;
    }


    return OK;
}