#include <stdio.h>
#include <unistd.h>

int main()
{
    int childpid;

    if ((childpid = fork())== -1)
    {
        perror("Can’t fork.\n");
        return 1;
    }
    else if (childpid == 0)
    {
        while (1) 
        {
            printf("%d  - i am child\n", getpid());
        }
    }
    else
    {
        while(1)
        {
            printf("%d - i am parent\n", getpid());
        }
    }

    return 0;
} 
