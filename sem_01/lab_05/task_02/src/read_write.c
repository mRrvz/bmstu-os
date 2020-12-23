#include "read_write.h"

struct sembuf READER_LOCK[] = { 
    { READ_QUEUE, 1, 0 }, 
    { ACTIVE_WRITER, 0, 0 }, 
    { WRITE_QUEUE, 0, 0 },
    { ACTIVE_READER, 1, 0 }, 
    { READ_QUEUE, -1, 0 }, 
};

struct sembuf READER_RELEASE[] = { 
    { ACTIVE_READER, -1, 0 }, 
};

struct sembuf WRITER_LOCK[] = {  
    { WRITE_QUEUE, 1, 0 }, 
    { ACTIVE_READER, 0, 0 }, 
    { ACTIVE_WRITER, 0, 0 }, 
    { ACTIVE_WRITER, 1, 0 }, 
    { WRITE_QUEUE, -1, 0 },
};

struct sembuf WRITER_RELEASE[] = { 
    { ACTIVE_WRITER, -1, 0 }, 
}; 

int start_read(int sid) {
    return semop(sid, READER_LOCK, 5) != -1;
}

int stop_read(int sid) {
    return semop(sid, READER_RELEASE, 1) != -1;
}

int start_write(int sid) {
    return semop(sid, WRITER_LOCK, 5) != -1;
}

int stop_write(int sid) {
    return semop(sid, WRITER_RELEASE, 1) != -1;
}

int reader_run(int *const shared_mem, const int sid, const int rid) {
    srand(time(NULL) + rid);

    if (!shared_mem) {
        return SHRDMEM_PTR_ERROR;
    }

    for (size_t i = 0; i < ITER_CNT; i++) {
        int sleep_time = rand() % TIME_RANGE + TIME_START;
        sleep(sleep_time);

        if (!start_read(sid)) {
            return READ_LOCK_ERROR;
        }

        int readed = *shared_mem;
        fprintf(stdout, "Reader %d read: %d\n", rid + 1, readed);

        if (!stop_read(sid)) {
            return READ_RELEASE_ERROR;
        }
    }

    return OK;
}

int writer_run(int *const shared_mem, const int sid, const int wid) {
    if (!shared_mem) {
        return SHRDMEM_PTR_ERROR;
    }

    for (size_t i = 0; i < ITER_CNT; i++) {
        int sleep_time = rand() % TIME_RANGE + TIME_START;
        sleep(sleep_time);

        if (!start_write(sid)) {
            return WRITE_LOCK_ERROR;
        }

        int updated = ++(*shared_mem);
        fprintf(stdout, "Writer %d write: %d\n", wid + 1, updated);

        if (!stop_write(sid)) {
            return WRITE_RELEASE_ERROR;
        }
    }

    return OK;
}