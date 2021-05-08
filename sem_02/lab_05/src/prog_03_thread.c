#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#define OK 0
#define FILE_NAME "data/out.txt"
#define SPEC "%c"

void *run_buffer(void *args)
{
    // fprintf(stdout, "\n============ IN RUN_BUFFER ==============");
    FILE *f = (FILE *)args;

    for (char c = 'b'; c <= 'z'; c += 2)
    {
        fprintf(f, SPEC, c);
    }

    fclose(f);
    // fprintf(stdout, "\n======== LEAVE RUN_BUFFER ==========");
    return NULL;
}

int main()
{
    FILE *f1 = fopen(FILE_NAME, "w");
    FILE *f2 = fopen(FILE_NAME, "w");

    pthread_t thread;
    int rc = pthread_create(&thread, NULL, run_buffer, (void *)(f2));

    for (char c = 'a'; c <= 'z'; c += 2)
    {
        fprintf(f1, SPEC, c);
    }

    pthread_join(thread, NULL);
    fclose(f1);

    return OK;
}