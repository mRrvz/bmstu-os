#include <stdio.h>
#include <fcntl.h>

#define OK 0
#define BUF_SIZE 20
#define VALID_READED 1

#define FILE_NAME "data/alphabet.txt"
#define SPEC "%c"

int main(void)
{
    int fd = open(FILE_NAME, O_RDONLY);

    FILE *fs1 = fdopen(fd, "r");
    char buff1[BUF_SIZE];
    setvbuf(fs1, buff1, _IOFBF, BUF_SIZE);

    FILE *fs2 = fdopen(fd, "r");
    char buff2[BUF_SIZE];
    setvbuf(fs2, buff2, _IOFBF, BUF_SIZE);

    int flag1 = 1, flag2 = 2;
    while (flag1 == VALID_READED || flag2 == VALID_READED)
    {
        char c;

        if ((flag1 = fscanf(fs1, SPEC, &c)) == VALID_READED)
        {
            fprintf(stdout, SPEC, c);
        }

        if ((flag2 = fscanf(fs2, SPEC, &c)) == VALID_READED)
        {
            fprintf(stdout, SPEC, c);
        }
    }

    return OK;
}
