#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define OK 0
#define VALID_READED 1
#define FILE_NAME "data/alphabet.txt"

void *run_buffer(void *args)
{
    // fprintf(stdout, "\n============ IN RUN_BUFFER ==============");
    int fd = *((int *)args);
    int err = VALID_READED;

    while (err == VALID_READED)
    {
        char c;
        err = read(fd, &c, 1);
        if (err == VALID_READED)
        {
            write(1, &c, 1);
        }
    }
    // fprintf(stdout, "\n======== LEAVE RUN_BUFFER ==========");

    return NULL;
}

int main(void)
{
    int fd1 = open(FILE_NAME, O_RDONLY);
    int fd2 = open(FILE_NAME, O_RDONLY);

    pthread_t thread;
    int rc = pthread_create(&thread, NULL, run_buffer, (void *)(&fd2));
    int err = VALID_READED;

    while (err == VALID_READED)
    {
        char c;
        err = read(fd1, &c, 1);
        if (err == VALID_READED)
        {
            write(1, &c, 1);
        }
    }

    pthread_join(thread, NULL);

    return OK;
}
