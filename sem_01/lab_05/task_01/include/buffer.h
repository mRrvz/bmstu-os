#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdio.h>

#define N 64

#define BUF_ERR -1
#define OK 0

typedef struct buf {
    size_t read_pos;
    size_t write_pos;
    char buffer[N];
} cbuffer_t;

int init_buffer(cbuffer_t *const buf);
int write_buffer(cbuffer_t *const buf, const char element);
int read_buffer(cbuffer_t *buf, char *const element);

#endif