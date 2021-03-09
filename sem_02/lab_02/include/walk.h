#ifndef __WALK_H__
#define __WALK_H__

#include <dirent.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "stack.h"

#define LSTAT_ERROR 4

int walkdir(char *pathname);

#endif