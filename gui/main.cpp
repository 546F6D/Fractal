#include <GL/glu.h>
#include <GL/glut.h>

/* size of screen and fractal 8 */
constexpr int WIDTH = 1400,	HEIGHT = 800;

/* normalized RGB values for one pixel */
struct rgb_t {
	double R, G, B;
};

/* pixel buffer */
rgb_t buf[WIDTH][HEIGHT];

void test_buffer()
{
	/* set everything to blue */
	for (int x = 0; x < WIDTH; ++x) {
		for (int y = 0; y < HEIGHT; ++y) {
			buf[x][y].R = 0;
			buf[x][y].G = 0;
			buf[x][y].B = 0.5;
		}
	}
}

void display()
{
	/* set state */
	glClearColor(1, 1, 1, 0);
	glColor3d(1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, WIDTH, 0, HEIGHT);
	glPointSize(5);
    glBegin(GL_POINTS);

	/* write pixel data to screen */
	for (int x = 0; x < WIDTH; ++x) {
		for (int y = 0; y < HEIGHT; ++y) {
			/* set color */
			rgb_t px = buf[x][y];
			glColor3d(px.R, px.G, px.B);

			/* set position */
			glVertex2i(x, y);
		}
	}

	/* cleanup */
    glEnd();
    glFlush();
}

int main(int argc, char **argv)
{
	test_buffer();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mandelbrot Fractal");
    glutDisplayFunc(display);
    glutMainLoop();
}

