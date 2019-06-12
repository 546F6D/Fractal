#include "fractal.h"
#include <stdio.h>
#include <stdlib.h>

void calc_fractal(int width, int height)
{
    /* calculate fractal */
    printf("Starting calculations (width=%d, height=%d)\n", width, height); 

    /* 2D array to hold pixel values */
    pixel_t *px;
    
    if (!(px = malloc(sizeof(pixel_t) * width * height))) {
        fprintf(stderr, "Failed to allocate memory for pixel data\n");
        exit(1);
    }

    /* calculate pixel colors */
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            /* x value between -2.0 and 1.0 */
            double norm_x = (double)x / width * 3.0 - 2.0;

            /* y value between -1.0 and 1.0 */
            double norm_y = (double)y / height * 2.0 - 1.0;

            /* calculate color and store in array */
            px[x * height + y] = calc_pixel(norm_x, norm_y);
        }
    }

    /* open PPM file and write pixel values */
    printf("Writing pixel values\n");

    FILE *out;
   
    if (!(out = fopen("fractal.ppm", "wb"))) {
        fprintf(stderr, "Failed to open file for writing\n");
        exit(1);
    }

    /* http://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C */
    fprintf(out, "P6\n%d %d\n255\n", width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            fwrite(px[x * height + y].data, 1, 3, out);
        }
    }

    /* clean up */ 
    fclose(out);
    free(px);

    /* convert to ppm to png using imagemagick-6.q16 */
    printf("Converting to png format\n");
    system("convert fractal.ppm fractal.png");
    printf("Conversion complete, see fractal.png\n");
}

pixel_t calc_pixel(double x0, double y0)
{
    /* https://en.wikipedia.org/wiki/Mandelbrot_set#Escape_time_algorithm */
    double x = 0.0;
    double y = 0.0;
    int iteration = 0;
    int max_iteration = MAX_ITERATIONS;
    while (x*x + y*y <= 2*2 && iteration < max_iteration) {
        double xtemp = x*x - y*y + x0;
        y = 2.0*x*y + y0;
        x = xtemp;
        iteration = iteration + 1;
    }

    /* translate iteration count to RGB value */
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 
        (unsigned char)((double)iteration / MAX_ITERATIONS * 255);

    /* return struct with RGB values */
    pixel_t px = { r, g, b };
    return px;
}

