#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define OK 0
#define FILE_NAME "data/out.txt"
#define SPEC "%c"

int main()
{
    FILE *f1 = fopen(FILE_NAME, "w");
    FILE *f2 = fopen(FILE_NAME, "w");

    for (char c = 'a'; c <= 'z'; c++)
    {
        if (c % 2)
        {
            fprintf(f1, SPEC, c);
        }
        else
        {
            fprintf(f2, SPEC, c);
        }
    }

    fclose(f2);
    fclose(f1);

    return OK;
}