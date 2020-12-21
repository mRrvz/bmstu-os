#include "consumer.h"

struct sembuf CONS_LOCK[] = { 
    { BUFF_FULL, -1, 0 }, 
    { BIN_SEM, -1, 0 } 
};

struct sembuf CONS_RELEASE[] = { 
    { BUFF_EMPTY, 1, 0 }, 
    { BIN_SEM, 1, 0 } 
};

int consumer_run(cbuffer_t *const buf, const int sid, const int consid) {
    srand(time(NULL) + consid + 3);

    if (!buf) {
        return BUF_ERR;
    }

    for (int i = 0; i < ITER_CNT; i++) {
        int sleep_time = rand() % CONS_TIME_RANGE + CONS_TIME_START;
        sleep(sleep_time);

        if (-1 == semop(sid, CONS_LOCK, SEM_SIZE)) {
            
            return CONS_LOCK_ERROR;
        }

        char symb;

        if (-1 == read_buffer(buf, &symb)) {
            return BUFF_READ_ERROR;
        }

        fprintf(stdout, "Consumer %d read: %c\n", consid + 1, symb);

        if (-1 == semop(sid, CONS_RELEASE, SEM_SIZE)) {
            return CONS_RELEASE_ERROR;
        }
    }

    return OK;
}