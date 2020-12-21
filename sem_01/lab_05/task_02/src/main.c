#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> 
#include <wait.h>

#include "read_write.h"

#define PERMISSIONS S_IRWXU | S_IRWXG | S_IRWXO
#define SHMAT_ERR_RET (void *)-1

#define READERS_CNT 5
#define WRITERS_CNT 3
#define SEM_CNT 4

#define OK 0
#define SHMGET_ERROR 1
#define SHMAT_ERROR 2
#define SEMGET_ERROR 3
#define FORK_ERROR 4
#define WAIT_ERROR 5
#define SHUTDOWN_ERROR 6

int main() {
    setbuf(stdout, NULL);

    int fd = shmget(IPC_PRIVATE, sizeof(int), PERMISSIONS | IPC_CREAT);
    if (-1 == fd) {
        perror("Error while creating shared memory.\n");
        return SHMGET_ERROR;
    }

    int *shared_mem_ptr = shmat(fd, 0, 0);
    if (SHMAT_ERR_RET == shared_mem_ptr) {
        perror("Error while creating shmat.\n");
        return SHMAT_ERROR;
    }

    int sid = semget(IPC_PRIVATE, SEM_CNT, PERMISSIONS | IPC_CREAT);
    if (-1 == sid) {
        perror("Error while creating array of semaphores.\n");
        return SEMGET_ERROR;
    }

    semctl(sid, ACTIVE_READER, SETVAL, 0);
    semctl(sid, ACTIVE_WRITER, SETVAL, 0);
    semctl(sid, WRITE_QUEUE, SETVAL, 0);
    semctl(sid, READ_QUEUE, SETVAL, 0);

    for (size_t i = 0; i < READERS_CNT; i++) {
        int child_pid = fork();

        if (-1 == child_pid) {
            perror("Error while fork (reader).");
            return FORK_ERROR;
        } else if (0 == child_pid) {
            reader_run(shared_mem_ptr, sid, i);
            return OK;
        }
    }

    for (size_t i = 0; i < WRITERS_CNT; i++) {
        int child_pid = fork();

        if (-1 == child_pid) {
            perror("Error while fork (reader).");
            return FORK_ERROR;
        } else if (0 == child_pid) {
            writer_run(shared_mem_ptr, sid, i);
            return OK;
        }
    }

    for (size_t i = 0; i < READERS_CNT + WRITERS_CNT; i++) {
        int statval;
        
        if (-1 == wait(&statval)) {
            perror("Error with child process.\n");
            return WAIT_ERROR;
        }

        if (!WIFEXITED(statval)) {
            fprintf(stderr, "Children process %lu terminated abnormally.", i);
        }
    }

    if (-1 == shmdt((void *)shared_mem_ptr) || -1 == shmctl(fd, IPC_RMID, NULL) || -1 == semctl(sid, IPC_RMID, 0)) {
        perror("Error while shutdown.\n");
        return SHUTDOWN_ERROR;
    }

    return OK;
}