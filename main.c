#include <stdio.h>
#include <stdlib.h>
#include "fractal.h"

int main(int argc, char *argv[])
{
    /* print usage and exit */
    if (argc < 3) {
        printf("Usage: ./fractal.exe <width> <height>\n");
        return 1;
    }

    /* get width and height */
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    if (width < 1 || height < 1) {
        fprintf(stderr, "Invalid width or height\n");
        return 1;
    }

    /* calculate pixel values and write to file */
    calc_fractal(width, height);
}

