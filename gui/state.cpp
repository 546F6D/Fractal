#include "state.h"
#include "pixel.h"
#include <GL/glut.h>
#include <chrono>
#include <iomanip>
#include <iostream>

State state;

State::State()
	: zoom(1), cx(-0.75), cy(0), render(false)
{
}

void State::pan_right()
{
	cx += PAN_RATE * zoom;
	update();
}

void State::pan_left()
{
	cx -= PAN_RATE * zoom;
	update();
}

void State::pan_down()
{
	cy -= PAN_RATE * zoom;
	update();
}

void State::pan_up()
{
	cy += PAN_RATE * zoom;
	update();
}

void State::zoom_in()
{
	zoom /= ZOOM_RATE;
	update();
}

void State::zoom_out()
{
	zoom *= ZOOM_RATE;
	update();
}

double State::ax(double px)
{
	return mx + sx * px;	
}

double State::ay(double py)
{
	return my + sy * py;	
}

void State::update()
{
	/* start time */
	auto t0 = std::chrono::high_resolution_clock::now();

	/* set render flag */
	render = true;
	glutSetWindowTitle("Busy");

	/* delta from center to edge */
	double dx = zoom * RATIO;
	double dy = zoom;

	mx = cx - dx;
	my = cy - dy;
	
	sx = 2 * dx / WIDTH;
	sy = 2 * dy / HEIGHT;
	
	/* calculate new state */
	calc_pixels();

	/* request new render */
	glutPostRedisplay();		
	
	/* unset render flag */
	render = false;
	glutSetWindowTitle("Ready");
	
	/* log render time to console */
	auto t1 = std::chrono::high_resolution_clock::now();
	auto dt = 
		std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0);
	
	std::cout << "render: " << std::setprecision(2) << std::fixed 
		<< dt.count() << " s" << std::endl; 
}

