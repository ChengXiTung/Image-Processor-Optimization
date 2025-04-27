/*

Project 4: Optimization

DO NOT MODIFY THIS FILE!

Driver code for the rotate program.

Usage: ./rotate assets/<your ppm image>.ppm output/<optm ppm output>.ppm

*/

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "ppm.h"
#include "rotate.h"

// You may have seen this in a previous lab
typedef struct timespec timespec_t;
double get_elapsed_time_sec(const timespec_t *start, const timespec_t *end) {
    long start_nanos = (long) 1e9 * start->tv_sec + start->tv_nsec;
    long end_nanos = (long) 1e9 * end->tv_sec + end->tv_nsec;
    return (double) (end_nanos - start_nanos) / 1e9;
}

// Tests the optimized image rotation function against the base version.
// Inspired by checkasm from FFmpeg and other multimedia source code.
int correctness_test(char *input_path, char *output_path) {
    // Set return value to 0
    int retval = 0;
    // Load the image
    fb_t *image_base = load_image(input_path);
    // Copy the image to a new buffer
    fb_t *image_optm = create_framebuffer(image_base->wi, image_base->he);
    memcpy(image_optm->r, image_base->r, image_base->wi * image_base->he * sizeof(uint16_t));
    memcpy(image_optm->g, image_base->g, image_base->wi * image_base->he * sizeof(uint16_t));
    memcpy(image_optm->b, image_base->b, image_base->wi * image_base->he * sizeof(uint16_t));
    // Call the functions
    rotate_image_base(image_base);
    rotate_image_optm(image_optm);
    // If any of the channels exhibit a difference in values, it is incorrect
    if (memcmp(image_optm->r, image_base->r, image_base->wi * image_base->he * sizeof(uint16_t))) {
        printf("Difference in Red channel!\n");
        retval = 1;
    };
    if (memcmp(image_optm->g, image_base->g, image_base->wi * image_base->he * sizeof(uint16_t))) {
        printf("Difference in Green channel!\n");
        retval = 1;
    }
    if (memcmp(image_optm->b, image_base->b, image_base->wi * image_base->he * sizeof(uint16_t))) {
        printf("Difference in Blue channel!\n");
        retval = 1;
    }
    // Save optimized results
    FILE *f = fopen(output_path, "w+");
    write_header(f, image_optm->wi, image_optm->he);
    write_framebuffer(f, image_optm, image_optm->wi, image_optm->he);
    fclose(f);
    // Free used buffers
    free(image_optm->r);
    free(image_optm->g);
    free(image_optm->b);
    free(image_optm);
    free(image_base->r);
    free(image_base->g);
    free(image_base->b);
    free(image_base);
    return retval;
}

// Tests the speed of the optimized impl. against the base impl.
void speed_test(char *input) {
    // Variables
    timespec_t start;
    timespec_t end;
    double base_cpu_time;
    double optm_cpu_time;

    // Load input image
    fb_t *image = load_image(input);

    // // Base image flip
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for (int i = 0; i < 128; i++) {
        rotate_image_base(image);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    base_cpu_time = get_elapsed_time_sec(&start, &end);
    printf("Base FPS: %.4f\n", 128 / base_cpu_time);

    // Optimized image flip
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for (int i = 0; i < 128; i++) {
        rotate_image_optm(image);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    optm_cpu_time = get_elapsed_time_sec(&start, &end);
    printf("Optm FPS: %.4f\n", 128 / optm_cpu_time);

    free(image->r);
    free(image->g);
    free(image->b);
    free(image);

    return;
}

int main(int argc, char *argv[]) {
    // Print usage info
    if (argc != 3) {
        printf("Usage: ./rotate <input.ppm> <output_base.ppm>\n");
        return -1;
    }
    printf("%s\n", correctness_test(argv[1], argv[2]) ? "Rotation incorrect" : "Rotation correct");
    speed_test(argv[1]);
    return 0;
}
