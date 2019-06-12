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

/* holds escape parameters for a pixel */
typedef struct Calculation {
	int iterations;
	double x, y;
	double norm_iter, next_iter;
} calc_t;

/* calculates and writes pixel values to file */
void calc_fractal(int width, int height);

/* calculate escape parameters for a pixel in mandelbrot set */ 
calc_t calc_pixel(double x0, double y0);

/* calculates map for translating iteration count to a normalized value */ 
double *calc_map(int *pop_cnt, int pixels);

/* translate normalized iterations to RGB */
pixel_t calc_rgb(double norm_iter);

/* find RGB and reduce aliasing for pixel */
pixel_t anti_alias(calc_t calc);

/* linear interpolate two RGB values */
unsigned char linear_inter(unsigned char v0, unsigned char v1, double t);

