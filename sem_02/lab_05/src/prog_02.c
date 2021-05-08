#include <fcntl.h>
#include <unistd.h>

#define OK 0
#define VALID_READED 1
#define FILE_NAME "data/alphabet.txt"

int main(void)
{
    int fd1 = open(FILE_NAME, O_RDONLY);
    int fd2 = open(FILE_NAME, O_RDONLY);
    int rc1, rc2 = VALID_READED;

    while (rc1 == VALID_READED || rc2 == VALID_READED)
    {
        char c;

        rc1 = read(fd1, &c, 1);
        if (rc1 == VALID_READED)
        {
            write(1, &c, 1);
        }

        rc2 = read(fd2, &c, 1);
        if (rc2 == VALID_READED)
        {
            write(1, &c, 1);
        }
    }

    return OK;
}