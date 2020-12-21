#include "producer.h"

struct sembuf PROD_LOCK[] = { 
    { BUFF_EMPTY, -1, 0 }, 
    { BIN_SEM, -1, 0 } 
};

struct sembuf PROD_RELEASE[] = { 
    { BUFF_FULL, 1, 0 }, 
    { BIN_SEM, 1, 0 } 
};

int producer_run(cbuffer_t *const buf, const int sid, const int prodid) {
    srand(time(NULL) + prodid);

    if (!buf) {
        return BUF_ERR;
    }

    for (size_t i = 0; i < ITER_CNT; i++) {
        int sleep_time = rand() % PROD_TIME_RANGE + PROD_TIME_START;
        sleep(sleep_time);

        if (-1 == semop(sid, PROD_LOCK, SEM_SIZE)) {
            return PROD_LOCK_ERROR;
        }

        const char symb = (buf->write_pos % 26) + 'a';
        if (-1 == write_buffer(buf, symb)) {
            return BUFF_WRITE_ERROR;
        }

        fprintf(stdout, "Producer %lu write: %c\n", prodid + 1, symb);

        if (-1 == semop(sid, PROD_RELEASE, SEM_SIZE)) {
            return PROD_RELEASE_ERROR;
        }
    }

    return OK;
}