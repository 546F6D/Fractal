#include "pixel.h"
#include "state.h"
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <string>

void init()
{
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, WIDTH, 0, HEIGHT);
	glPointSize(1);
	state.update();
}

void close()
{
	glutDestroyWindow(glutGetWindow());
}

void display()
{
	/* set screen background color */
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	/* write pixel data to screen */
    glBegin(GL_POINTS);
	
	for (int x = 0; x < WIDTH; ++x) {
		for (int y = 0; y < HEIGHT; ++y) {
			/* set color */
			rgb_t px = buf[x][y];
			glColor3d(px.R, px.G, px.B);

			/* set position */
			glVertex2i(x, y);
		}
	}

    glEnd();
    glFlush();

	/* present buffer */
    glutSwapBuffers();
}

void special(int key, int x, int y)
{
	/* ignore additional requests while rendering */
	if (state.render) {
		return;
	}

	switch (key) {
	case GLUT_KEY_LEFT:
		state.pan_left();
		break;

	case GLUT_KEY_RIGHT:				
		state.pan_right();
		break;

	case GLUT_KEY_UP:				
		state.pan_up();
		break;

	case GLUT_KEY_DOWN:				
		state.pan_down();
		break;

	case GLUT_KEY_PAGE_UP:
		state.zoom_in();
		break;

	case GLUT_KEY_PAGE_DOWN:
		state.zoom_out();
		break;
	}
}

void read_cmd()
{
	std::cout << "$ ";
	std::string cmd;
	std::cin >> cmd;
	if (cmd == "set") {
		std::cin >> cmd;
		if (cmd == "red") {
			int hexval;
			std::cin >> hexval;
			state.R = hexval / 255.0;
			state.update();
		}
		else if (cmd == "green") {
			int hexval;
			std::cin >> hexval;
			state.G = hexval / 255.0;
			state.update();
		}
		else if (cmd == "blue") {
			int hexval;
			std::cin >> hexval;
			state.B = hexval / 255.0;
			state.update();
		}
		else if (cmd == "zoom") {
			double val;
			std::cin >> val;
			state.zoom = val;
			state.update();
		}
		else if (cmd == "cx") {
			double val;
			std::cin >> val;
			state.cx = val;
			state.update();
		}
		else if (cmd == "cy") {
			double val;
			std::cin >> val;
			state.cy = val;
			state.update();
		}
		else {
			std::cout << "no such command" << std::endl;
		}
	}
	else if (cmd == "exit") {
		close();
	}
	else {
		std::cout << "no such command" << std::endl;
	}
}	

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {

	/* space bar */
	case '\x20':
		read_cmd();
		break;

	/* escape */
	case '\x1B':
		close();
		break;	
	}		
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Loading");
    glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	init();
	glutMainLoop();
}


