#include "fractal.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calc_fractal(int width, int height)
{
    /* find the number of iterations to escape mandelbrot set */
    printf("Starting calculations\n");

    /* allocate memory for iteration population count used for normalization */
    int *pop_cnt;
    
    if (!(pop_cnt = malloc(sizeof(int) * MAX_ITERATIONS + 1))) {
        fprintf(stderr, "Failed to allocate memory for population count\n");
        exit(1);
    }

    /* zero memory */
    memset(pop_cnt, 0, sizeof(int) * MAX_ITERATIONS + 1);

    /* number of pixels inside the set that are included in the pop. count */ 
    int pixels = 0;

    /* allocate memory for iteration count for each pixel */
    int *iter_cnt;
    
    if (!(iter_cnt = malloc(sizeof(int) * width * height))) {
        fprintf(stderr, "Failed to allocate memory for iteration count\n");
        exit(1);
    }

	/* allocate memory for the escape parameters for each pixel */
	calc_t *px_calc;

	if (!(px_calc = malloc(sizeof(calc_t) * width * height))) {
        fprintf(stderr, "Failed to allocate memory for escape parameters\n");
        exit(1);
    }

    /* calculate iterations for each pixel */
    for (int px = 0; px < width; ++px) {
        for (int py = 0; py < height; ++py) {
            /* normalized x and y coordinate values */
            double x = (double)px / width * 3.0 - 2.0;
            double y = (double)py / height * 2.0 - 1.0;

			/* calculate and store escape parameters */ 
           	calc_t calc = px_calc[px * height + py] = calc_pixel(x, y);
            
			/* store num. of iterations */ 
			int iterations 
                = iter_cnt[px * height + py]
				= calc.iterations;

            /* pixels outside the set are not included in the pop. count */
            if (iterations < MAX_ITERATIONS) {

                /* sanity check array bounds */
                assert(0 <= iterations && iterations < MAX_ITERATIONS);
                
                /* increment number of pixels with given iteration count */
                pop_cnt[iterations]++;

                /* increment total number of pixels in pop. count */
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

            /* normalized iteration value */
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
		
			/* get pointer */
			calc_t *calc = px_calc + (px * height + py);

			/* store normalized iterations */
			calc->norm_iter = norm_iter;

			/* store next iteration step */
			calc->next_iter 
				= iter + 1 < MAX_ITERATIONS 
				? map[iter + 1] 
				: calc->norm_iter;	

			/* calculate RGB value */
			pixel_t pixel = anti_alias(*calc);

			/* store value */
            px_rgb[px * height + py] = pixel;
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
	free(px_calc);
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

calc_t calc_pixel(double x0, double y0)
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
    
	/* return calculation results */
	calc_t calc_res;
	calc_res.iterations = iteration;
	calc_res.x = x;
	calc_res.y = y;
    return calc_res;
}

pixel_t calc_rgb(double val)
{
	/* sanity check */
	assert(0.0 <= val && val <= 1.0);

    /* struct for holding RGB values */
    pixel_t px = { 0, 0, 0 };

    /* color pixels with value of 1.0 black */
    if (val == 1.0) {
        px.r = px.g = px.b = 0;
    }

    else {
		int step = (int)(val * 4.0);
		switch (step) {
		case 3:
			px.r = 0;
			px.g = (unsigned char)(180.0 * fmod(val * 4.0, 1.0));
			px.b = 0;
			break;

		case 2:
			px.r = 0;
			px.g = (unsigned char)(16.0 + 64.0 * fmod(val * 4.0, 1.0));
			px.b = 0;
			break;

		case 1:
			px.r = 0;
			px.g = (unsigned char)(32.0 * fmod(val * 4.0, 1.0));
			px.b = 0; 
			break;
		
		case 0:
			px.r = 0;
			px.g = 0; 
			px.b = 0;
			break;
		}
    }
    
    return px;
}

pixel_t anti_alias(calc_t calc) 
{	
	pixel_t color;
	
	if (calc.norm_iter < 1.0) {
		/* simple names for readability */
		double x = calc.x;
		double y = calc.y;

		/* https://en.wikipedia.org/wiki/Mandelbrot_set#Continuous_(smooth)_coloring */
		double log_zn = log(x*x	+ y*y) / 2.0;
		double nu = log(log_zn / log(2.0)) / log(2.0);
		double iteration = calc.iterations + 1.0 - nu;

		/* find adjacent color values */
//		unsigned char b1 = (unsigned char)(calc.norm_iter * 255.0);
//		pixel_t color1 = { 0, 0, b1 };
	
//		unsigned char b2 = (unsigned char)(calc.next_iter * 255.0);
//		pixel_t color2 = { 0, 0, b2 };

		pixel_t color1 = calc_rgb(calc.norm_iter);
		pixel_t color2 = calc_rgb(calc.next_iter);

		/* use linear interpolation to find final color */
		double t = fmod(iteration, 1.0);
		color.r = linear_inter(color1.r, color2.r, t);
		color.g = linear_inter(color1.g, color2.g, t);
		color.b = linear_inter(color1.b, color2.b, t);
	}

	/* color outside of set black */ 
	else {
		color.r = color.g = color.b = 0;
	}

	return color;
}

unsigned char linear_inter(unsigned char v0, unsigned char v1, double t)
{
	/* sanity check on t */
	assert(-1.0 <= t && t <= 1.0);

	/* https://en.wikipedia.org/wiki/Linear_interpolation#Programming_language_support */
	return (unsigned char)((1.0 - t) * v0 + t * v1);
}

