#include "pixel.h"
#include <cmath>

rgb_t buf[WIDTH][HEIGHT];

void calc_pixels()
{
	/* calculate new RGB values for each pixel */
	for (int px = 0; px < WIDTH; ++px) {
		for (int py = 0; py < HEIGHT; ++py) {
			buf[px][py] = calc_pixel(px, py);
		}
	}
}

rgb_t calc_pixel(double px, double py)
{
	/* translate pixel coordinates to actual coordinates */
	double ax = state.ax(px);
	double ay = state.ay(py);

	double x = 0;
	double y = 0;
	int i = 0;	

	/* escape time algorithm */
	while (pow(x, 2) + pow(y, 2) < THRESHOLD && i < MAX_ITER) {
		double xtemp = pow(x, 2) - pow(y, 2) + ax;
		y = 2*x*y + ay;
		x = xtemp;
		++i;
	}

	/* color inside pixels black */
	if (i == MAX_ITER) {
		return { 0, 0, 0 };
	}

	/* no interpolation for last bucket */
	if (i + 1 == MAX_ITER) {
		return color(i);
	}

	/* smooth coloring algorithm */
	double zn = log(pow(x, 2) + pow(y, 2)) / 2;
	double nu = log(zn / log(2)) / log(2);
	double it = i + 1 - nu;

	/* end points */
	rgb_t a = color(floor(it));
	rgb_t b = color(floor(it) + 1);

	/* interpolated color */
	return {
		linear(a.R, b.R, fmod(it, 1)),
		linear(a.G, b.G, fmod(it, 1)),
		linear(a.B, b.B, fmod(it, 1))
	};
}

rgb_t color(double a)
{
	return { a * 35, a * 35, a * 200 };
}

double linear(double a, double b, double t)
{
	double u = 1 - t;
	return a*u + b*t;
}
