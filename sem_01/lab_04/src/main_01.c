#include <stdio.h>
#include <unistd.h>

#define OK 0
#define FORK_FAILURE 1

#define N 2
#define SLP_INTV 2

int main() 
{
    int child[N];

    fprintf(stdout, "Parent process. PID: %d, GROUP: %d\n", getpid(), getpgrp());

    for (int i = 0; i < N; i++) 
    {
        int pid = fork();

        if (-1 == pid) 
        {
            perror("Cant fork.");
            return FORK_FAILURE;
        } 
        else if (0 == pid) 
        {
            sleep(SLP_INTV);
            fprintf(stdout, "Child process #%d. PID: %d, PPID: %d, GROUP: %d\n", i + 1, getpid(), getppid(), getpgrp());
            return OK;
        } else 
        {
            child[i] = pid;
        }
    }

    fprintf(stdout, "Parent process. Children ID: %d, %d.\nParent process is dead.\n", child[0], child[1]);

    return OK;
}
