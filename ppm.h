#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PIXEL_H
#define PIXEL_H

typedef struct framebuffer {
    uint16_t *r;
    uint16_t *g;
    uint16_t *b;
    int wi;
    int he;
} fb_t;

void set_pixel(fb_t *, uint64_t n, uint16_t, uint16_t, uint16_t);
fb_t *create_framebuffer(int, int);
void write_framebuffer(FILE *, fb_t *, int, int);
void write_header(FILE *, int, int);
fb_t *load_image(char *);

#endif    // PIXEL_H
