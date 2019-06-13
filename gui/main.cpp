#include "pixel.h"
#include "state.h"
#include <GL/glu.h>
#include <GL/glut.h>

void init()
{
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, WIDTH, 0, HEIGHT);
	glPointSize(5);
	state.update();
}

void idle()
{
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

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Loading");
    glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutSpecialFunc(special);
	init();
	glutMainLoop();
}

