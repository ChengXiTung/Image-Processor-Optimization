/*

CSCI 2021 Project 4: Optimization

DO NOT MODIFY THIS FILE

This is a "correct" implementation of an image rotation function.
Your optimized version will be compared against this implementation.
Use this implementation as a reference, but do not modify it.

*/

#include "rotate.h"

uint16_t get_pixel_value(fb_t *fb, int x, int y, char c) {
    switch (c) {
        case 'R':
        case 'r':
            return fb->r[y * fb->wi + x];
        case 'G':
        case 'g':
            return fb->g[y * fb->wi + x];
        case 'B':
        case 'b':
            return fb->b[y * fb->wi + x];
        default:
            return 0;
    }
}

void set_pixel_value(fb_t *fb, int x, int y, char c, uint16_t val) {
    switch (c) {
        case 'R':
        case 'r':
            fb->r[y * fb->wi + x] = val;
            return;
        case 'G':
        case 'g':
            fb->g[y * fb->wi + x] = val;
            return;
        case 'B':
        case 'b':
            fb->b[y * fb->wi + x] = val;
            return;
    }
}

void rotate_image_base(fb_t *fb) {
    // Create a copy of the framebuffer
    fb_t *original = create_framebuffer(fb->wi, fb->he);
    memcpy(original->r, fb->r, fb->wi * fb->he * sizeof(uint16_t));
    memcpy(original->g, fb->g, fb->wi * fb->he * sizeof(uint16_t));
    memcpy(original->b, fb->b, fb->wi * fb->he * sizeof(uint16_t));
    for (int i = 0; i < fb->wi; i++) {
        for (int j = 0; j < fb->he; j++) {
            // PIX_R(fb, i, j) = PIX_R(original, (original->wi - 1 - i), (original->he - 1 - j));
            // PIX_G(fb, i, j) = PIX_G(original, (original->wi - 1 - i), (original->he - 1 - j));
            // PIX_B(fb, i, j) = PIX_B(original, (original->wi - 1 - i), (original->he - 1 - j));
            set_pixel_value(
                fb, i, j, 'R',
                get_pixel_value(original, (original->wi - 1 - i), (original->he - 1 - j), 'R'));
            set_pixel_value(
                fb, i, j, 'G',
                get_pixel_value(original, (original->wi - 1 - i), (original->he - 1 - j), 'G'));
            set_pixel_value(
                fb, i, j, 'B',
                get_pixel_value(original, (original->wi - 1 - i), (original->he - 1 - j), 'B'));
        }
    }
    free(original->r);
    free(original->g);
    free(original->b);
    free(original);
    return;
}
