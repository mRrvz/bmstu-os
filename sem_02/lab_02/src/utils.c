#include "utils.h"

int myfunc(const char *const filename, size_t offset)
{
    fprintf(stdout, " ");

    for (size_t i = 0; i < offset; i++)
    {
        fprintf(stdout, "\033[1;31m====> \033[0m");
    }

    if (!offset)
    {
        fprintf(stdout, "\033[1;34mROOT:\033[0m %s\n", filename);
    }
    else
    {
        fprintf(stdout, "%s\n", filename);
    }

    return OK;
}

char *joinpath(const char *base, const char *const file)
{
    int len = strlen(base);
    char *path = malloc(len + strlen(file) + 2);

    if (!path)
    {
        exit(ALLOCATION_ERROR);
    }

    strcpy(path, base);
    path[len] = '/';
    strcpy(path + len + 1, file);

    return path;
}

char *get_dynamic_str(const char *const string, const size_t size)
{
    char *dynamic_str = malloc(size + 1);

    if (!dynamic_str)
    {
        exit(ALLOCATION_ERROR);
    }

    strcpy(dynamic_str, string);
    return dynamic_str;
}
