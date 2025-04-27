/*

CSCI 2021 Project 4: Optimization

DO NOT MODIFY THIS FILE

This implements a set of functions that allow us to create, load,
and manipulate .ppm files.

*/

#include "ppm.h"

void pix_to_string(fb_t *fb, char *buf, uint64_t n) {
    sprintf(buf, "%5d %5d %5d ", fb->r[n], fb->g[n], fb->b[n]);
}

inline void set_pixel(fb_t *fb, uint64_t n, uint16_t red, uint16_t green, uint16_t blue) {
    /* I wonder what inline means... */
    fb->r[n] = red;
    fb->g[n] = green;
    fb->b[n] = blue;
}

// Allocate memory for a new framebuffer.
// Usage:   int w - Width of image
//          int h - Height of image
fb_t *create_framebuffer(int w, int h) {
    fb_t *fb = malloc(sizeof(fb_t));
    if (fb == NULL) {
        printf("Memory allocation error. Exiting...\n");
        exit(1);
    }
    /*
        The r, g, b arrays are aligned not only to 16 bytes but also to 32.
        I wonder what kind of instructions can be aligned to 32..
     */
    if ((posix_memalign((void **) &fb->r, 32, w * h * sizeof(uint16_t)) != 0) ||
        (posix_memalign((void **) &fb->g, 32, w * h * sizeof(uint16_t)) != 0) ||
        (posix_memalign((void **) &fb->b, 32, w * h * sizeof(uint16_t)) != 0)) {
        printf("Memory allocation error. Exiting...\n");
        exit(1);
    }
    fb->wi = w;
    fb->he = h;
    return fb;
}

// Write the contents of a framebuffer to the specified file
void write_framebuffer(FILE *f, fb_t *fb, int w, int h) {
    int n = 0;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            char s[24];
            pix_to_string(fb, s, (y * w) + x);
            fprintf(f, "%s", s);
            n++;
            if (n == 6) {
                n = 0;
                fprintf(f, "\n");
            }
        }
    }
}

// Initialize a file with PPM magic numbers
void write_header(FILE *f, int w, int h) {
    fprintf(f, "P3\n");
    fprintf(f, "%d %d\n", w, h);
    fprintf(f, "65535\n");
}

// Fully read in a PPM image. Not fully conformant to the
// PPM spec. Requires no comments. imagemagick can generate
// these with:
//      magick convert <whatever input> -depth 16 -compress None <output>.ppm
fb_t *load_image(char *filename) {
    FILE *texture_file = fopen(filename, "r");
    if (texture_file == NULL) {
        printf("Error opening file.");
        exit(1);
    }
    char header_number[3];
    int width, height, max_color_value;
    fscanf(texture_file, "%2s", header_number);
    if (strcmp(header_number, "P3")) {
        printf("Bad PPM file: Bad header");
        exit(1);
    }
    fscanf(texture_file, "%d %d %d", &width, &height, &max_color_value);
    if (max_color_value != 65535) {
        printf("Bad PPM file: Bad max %d\n", max_color_value);
        exit(1);
    }
    int acc = 0;
    fb_t *buf = create_framebuffer(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint16_t r, g, b;
            if (fscanf(texture_file, "%hu %hu %hu", &r, &g, &b) == EOF) {
                break;
            }
            set_pixel(buf, y * width + x, r, g, b);
            acc += 1;
        }
    }
    if (acc != width * height) {
        printf("Warning: malformed texture PPM not correctly read\n");
    }
    fclose(texture_file);
    return buf;
}
