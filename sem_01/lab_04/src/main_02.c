#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define OK 0
#define FORK_FAILURE 1

#define N 2
#define SLP_INTV 2

int main() 
{
    int child[N];
    int pid;

    fprintf(stdout, "Parent process. PID: %d, GROUP: %d\n", getpid(), getpgrp());

    for (size_t i = 0; i < N; i++) 
    {
        pid = fork();

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

    for (size_t i = 0; i < N; i++) 
    {
        int status, statval;

        pid_t childpid = wait(&status);
        fprintf(stdout, "Child process (PID %d) finished. Status: %d\n", childpid, status);

        if (WIFEXITED(statval)) 
        {
            fprintf(stdout, "Child process finished with code: %d\n", WEXITSTATUS(statval));
        }
        else 
        {
            fprintf(stdout, "Child process terminated abnormally\n");
        }

    }

    fprintf(stdout, "Parent process. Children ID: %d, %d.\nParent process is dead.\n", child[0], child[1]);

    return OK;
}
