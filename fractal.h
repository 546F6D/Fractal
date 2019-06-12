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

/* calculates color for a pixel given normalized coordinates */
pixel_t calc_pixel(double x0, double y0);


