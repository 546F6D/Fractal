#pragma once
#include "state.h"

/* escape time algorithm parameters */
constexpr double THRESHOLD = 4.0;
constexpr int MAX_ITER = 1000;

/* normalized RGB values for one pixel */
struct rgb_t {
	double R, G, B;
};

/* pixel buffer */
extern rgb_t buf[WIDTH][HEIGHT];

/* set buffer according to current state */
void calc_pixels();

/* find RGB value for pixel */
rgb_t calc_pixel(double px, double py);

/* retrieve color for normalized value */
rgb_t color(double a);

/* linear interpolation */
double linear(double a, double b, double t);

