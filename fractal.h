#pragma once

/* max. number of iterations for mandelbrot algorithm */
#define MAX_ITERATIONS (1000)

/* holds value between 0-255 for each RGB channel */
typedef struct Pixel { 
    union {
        struct { unsigned char r, g, b; };
        unsigned char data[3];
    };
} pixel_t;

/* calculates and writes pixel values to file */
void calc_fractal(int width, int height);

/* calculates number of iterations for pixel to escape mandelbrot set */ 
int calc_iterations(double x0, double y0);

/* calculates map for translating iteration count to a normalized value */ 
double *calc_map(int *pop_cnt, int pixels);

/* translate normalized value to RGB */
pixel_t calc_rgb(double norm_iter);


