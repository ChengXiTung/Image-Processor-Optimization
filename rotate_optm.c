/*

CSCI 2021 Project 4: Optimization

MODIFY THIS FILE

This is where your optimized implementation of the rotate function is.

*/

#include "rotate.h"

// Hmm... So instead of creating a copy, we can just reverse the order of the pixels in each colr array
// directly. For example, if the original image is stored in an array of pixels, we can just swap the first
// pixel with the last pixel, the second pixel with the second-to-last pixel, and so on. That way, we don't
// need to create a new array and copy the pixels over, which saves memory and time

// So to prove why this is more efficient, we can take the cache approach, when we access elements
// sequentially, CPU can load blocks of memory into the cache and make the subsequent accesses faster, the
// base code jumps around in memory, casued more cache misses and slower performance while the optimized code
// accesses the elements in a more linear fashion, which allows the CPU to prefetch the data into the cache
// more effectively

// Original code:
// 1. Uses nested loops, which typically takes O(n^2) to swap the pixels in place -> less efficient than using a single loop.
// 2. Creates a copy of the frame buffer, uses more memory and is slower.
// 3. Accesses the pixels in a non-linear fashion, which can cause cache misses and slow down performance.
// 4. Too many function calls to get and set pixel value.

// Optimized code:
// 1. Uses one single while loop to swap the pixels in place, which is more efficient than using nested loops.
// 2. Avoids creating a copy of the framebffer, which saves memory and time.
// 3. Accesses the pixels in a linear fashion -> allows the CPU to prefetch the data into the cache
// 4. Less function calls to get and set pixel values -> reduces overhead
// 5. Uses local variables to store the pixel values -> reduces the number of memory accesses.
// 6. Uses registers for temporary variables.
// 

void rotate_image_optm(fb_t *fb) {
    int num_pixels = fb->wi * fb->he;
    uint16_t *r = fb->r;
    uint16_t *g = fb->g;
    uint16_t *b = fb->b;
    int i = 0;
    int j = num_pixels - 1;

    while (i < j) {
        // Swap red channel elements
        uint16_t temp_r = r[i];
        r[i] = r[j];
        r[j] = temp_r;

        // Swap green channel elements
        uint16_t temp_g = g[i];
        g[i] = g[j];
        g[j] = temp_g;

        // Swap blue channel elements
        uint16_t temp_b = b[i];
        b[i] = b[j];
        b[j] = temp_b;

        // Move to the next pair of elements
        i++;
        j--;
    }
}
