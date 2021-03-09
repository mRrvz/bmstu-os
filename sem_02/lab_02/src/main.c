#include <stdio.h>
#include "walk.h"

#define ARGS_ERROR 3

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "You should choose directory to be crawled.");
        return ARGS_ERROR;
    }

    return walkdir(argv[1]);
}