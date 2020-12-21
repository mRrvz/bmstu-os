#include "buffer.h"

int init_buffer(cbuffer_t *const buf) {
    if (!buf) {
        perror("Error while initializing buffer.\n");
        return BUF_ERR;
    }

    return OK;
}

int write_buffer(cbuffer_t *const buf, const char element) {
    if (!buf) {
        return BUF_ERR;
    }

    buf->buffer[buf->write_pos++] = element;
    buf->write_pos %= N;

    return OK;
}

int read_buffer(cbuffer_t *buf, char *const element) {
    if (!buf) {
        return BUF_ERR;
    }

    if (!element) {
        return BUF_ERR;
    }

    *element = buf->buffer[buf->read_pos++];
    buf->read_pos %= N;

    return OK;
}