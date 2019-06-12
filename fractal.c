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

			/* store prev/next iteration step */
			calc->prev_iter 
				= iter - 1 >= 0 
				? map[iter - 1] 
				: 0.0;	

			calc->next_iter
				= iter + 1 < MAX_ITERATIONS
				? map[iter + 1]
				: 1.0;
			

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
    while (x*x + y*y <= 4.0 && iteration < max_iteration) {
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
	/* ensure val is between 0.0 and 1.0
	if (val < 0.0) {
		val = 0.0;
	}
	else if (val > 1.0) {
		val = 1.0;
	}

    /* struct for holding RGB values */
    pixel_t px = { 0, 0, 0 };

	int step = (int)((1.0 - val) * 6.0);
	switch (step) {
	case 0:
		px.r = (unsigned char)(200.0 + 20.0 * fmod(val * 6.0, 1.0));
		px.g = (unsigned char)(40.0 + 180.0 * fmod(val * 6.0, 1.0));
		px.b = (unsigned char)(100.0 + 10.0 * fmod(val * 6.0, 1.0));
		break;
		
	case 1:
		px.r = (unsigned char)(80.0 + 120.0 * fmod(val * 6.0, 1.0));
		px.g = (unsigned char)(25.0 + 15.0 * fmod(val * 6.0, 1.0));
		px.b = (unsigned char)(70.0 + 30.0 * fmod(val * 6.0, 1.0));
		break;

	case 2:
		px.r = (unsigned char)(25.0 + 55.0 * fmod(val * 6.0, 1.0));
		px.g = 25;
		px.b = 70;
		break;
	
	case 3:
		px.r = (unsigned char)(15.0 + 10.0 * fmod(val * 6.0, 1.0));
		px.g = (unsigned char)(15.0 + 10.0 * fmod(val * 6.0, 1.0));
		px.b = (unsigned char)(65.0 + 5.0 * fmod(val * 6.0, 1.0));
		break;
	
	case 4:
		px.r = 15; 
		px.g = 15;
		px.b = 65;
		break;

	default:
		px.r = 15;
		px.g = 15;
		px.b = 65;
		break;
	}
    
    return px;
}

pixel_t anti_alias(calc_t calc) 
{
	if (calc.iterations == MAX_ITERATIONS) {
		pixel_t pixel = { 0, 0, 0 };
		return pixel;
	}
	
	double A 
		= -1.0 
		* log(log(sqrt(pow(calc.x, 2.0) + pow(calc.y, 2.0)))) 
		/ log(2.0);

	double B = (calc.next_iter - calc.prev_iter) / 2;
	double C = A * B + calc.norm_iter;

	return calc_rgb(C);
}


