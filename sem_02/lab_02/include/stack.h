#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define STACK_ERROR 2

typedef struct item {
    char *dir;
    size_t level;
} item_t;

typedef struct stack {
    item_t *arr;
    size_t capacity;
    size_t size;
} stack_t;


stack_t init();

void push(stack_t *const stack, item_t elem);

item_t pop(stack_t *const stack);

void free_stack(const stack_t *const stack);

#endif
