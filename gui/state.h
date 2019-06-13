#pragma once

/* size of screen */
constexpr int WIDTH = 1400, HEIGHT = 800;
//constexpr int WIDTH = 300, HEIGHT = 200;

constexpr double RATIO = static_cast<double>(WIDTH) / HEIGHT;

constexpr double PAN_RATE = 0.1, ZOOM_RATE = 1.1;

struct State {
	State();

	void pan_right();
	void pan_left();
	void pan_down();
	void pan_up();
	void zoom_in();
	void zoom_out();

	/* update state variables and redraw screen */
	void update();

	/* translate pixel coordinates to actual coordinates */
	double ax(double px);
	double ay(double py);

	double zoom;

	/* center of screen */ 
	double cx;
	double cy;

	/* min. x and y */
	double mx;
	double my;

	/* screen step size for x and y */
	double sx;
	double sy;

	/* flag set when rendering */ 
	bool render;
};

extern State state;

