#include "fractal.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calc_fractal(int width, int height)
{
    /* first, find the number of iterations to escape mandelbrot set
       for each pixel */
    printf("Starting calculations\n");

    /* allocate memory for iteration population count used for normalization */
    int *pop_cnt;
    
    if (!(pop_cnt = malloc(sizeof(int) * MAX_ITERATIONS + 1))) {
        fprintf(stderr, "Failed to allocate memory for population count\n");
        exit(1);
    }

    /* zero memory */
    memset(pop_cnt, 0, sizeof(int) * MAX_ITERATIONS + 1);

    /* number of pixel included in count */ 
    int pixels = 0;

    /* allocate memory for iteration count for each pixel */
    int *iter_cnt;
    
    if (!(iter_cnt = malloc(sizeof(int) * width * height))) {
        fprintf(stderr, "Failed to allocate memory for iteration count\n");
        exit(1);
    }

    /* calculate iterations to escape set for each pixel */
    for (int px = 0; px < width; ++px) {
        for (int py = 0; py < height; ++py) {
            /* find normalized x and y coordinate values */
            double x = (double)px / width * 3.0 - 2.0;
            double y = (double)py / height * 2.0 - 1.0;

            /* number of iterations for current pixel */
            int iterations 
                = iter_cnt[px * height + py] 
                = calc_iterations(x, y);

            /* pixels outside the set are not included in the count */
            if (iterations < MAX_ITERATIONS) {

                /* sanity check array bounds */
                assert(0 <= iterations && iterations < MAX_ITERATIONS);
                
                /* increment population for that iteration count */
                pop_cnt[iterations]++;

                /* increment number of pixels in total population */
                pixels++;
            }
        }
    }

    /* calculate normalized iteration translation map */
    double *map = calc_map(pop_cnt, pixels); 

    /* allocate memory to hold pixel RGB values */
    pixel_t *px_rgb;
    
    if (!(px_rgb = malloc(sizeof(pixel_t) * width * height))) {
        fprintf(stderr, "Failed to allocate memory for pixel RGB values\n");
        exit(1);
    }

    /* calculate RGB values for each pixel */
    printf("Finding RGB values\n");
    for (int px = 0; px < width; ++px) {
        for (int py = 0; py < height; ++py) {
            /* iterations for current pixel */ 
            int iter = iter_cnt[px * height + py];

            /* find normalized iteration value */
            double norm_iter;

            if (iter == MAX_ITERATIONS) {
                norm_iter = 1.0;
            }
            else {
                /* sanity check array bounds */
                assert(0 <= iter && iter < MAX_ITERATIONS);

                /* use translation map */
                norm_iter = map[iter];
            }

            /* calculate and store RGB value */
            px_rgb[px * height + py] = calc_rgb(norm_iter);
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

    for (int py = 0; py < height; ++py) {
        for (int px = 0; px < width; ++px) {
            fwrite(px_rgb[px * height + py].data, 1, 3, out);
        }
    }
    
    /* close ppm file */
    fclose(out);

    /* convert to ppm to png using imagemagick-6.q16 */
    printf("Converting to png format\n");
    system("convert fractal.ppm fractal.png");
    printf("Conversion complete, see fractal.png\n");
    
    /* free memory */ 
    free(px_rgb);
    free(pop_cnt);
    free(iter_cnt);
    free(map);
}

double *calc_map(int *pop_cnt, int pixels)
{
    /* allocate memory for map */
    double *map;

    if (!(map = malloc(sizeof(double) * MAX_ITERATIONS))) {
        fprintf(stderr, "Failed to allocate memory for map\n");
        exit(1);
    }

    /* calculate normalized translation values */
    double cur_val = 0.0;
    for (int i = 0; i < MAX_ITERATIONS; ++i) {
        map[i] = cur_val;

        /* calculate value increase by for next iteration */
        double step = (double)pop_cnt[i] / pixels;

        /* increase current normalized translation value */
        cur_val += step;
    }


    return map;
}

int calc_iterations(double x0, double y0)
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
    
    /* return number of iterations for pixel to escape mandelbrot set */
    return iteration;
}

pixel_t calc_rgb(double norm_iter)
{
    /* struct for holding RGB values */
    pixel_t px = { 0, 0, 0 };

    /* color pixels with value of 1.0 black */
    if (norm_iter == 1.0) {
        px.r = px.g = px.b = 0;
    }

    else {
        px.r = 0;
        px.g = 0;
        px.b = (unsigned char)(norm_iter * 255.0);
    }
    
    return px;
}
