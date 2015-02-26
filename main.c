#include <stdio.h>
#include <math.h>

#include "tga.h"

void swap(int *a, int *b);
int abs(int a);

int sign(int a);
/*
* Using Bresenham algorihm
* to draw interval connecting (x0, y0) with (x1, y1)
* on image using color
*/
void line (tgaImage *image, 
           int x0, int y0,
           int x1, int y1,
           tgaColor color);

int main(int argc, char **argv)
{
    int rv = 0;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s outfile\n", argv[0]);
        return -1;
    }
    tgaImage * image = tgaNewImage(800, 800, RGB);

    line(image, 10, 10, 600, 250, tgaRGB(255, 255, 255));
    if (-1 == tgaSaveToFile(image, argv[1])) {
        perror("tgaSateToFile");
        rv = -1;
    }

    tgaFreeImage(image);    
    return rv;
}

void line (tgaImage *image, 
           int x0, int y0,
           int x1, int y1,
           tgaColor color)
{
    int transpose = 0;
    if (abs(y1 - y0) > abs(x1 - x0)) {
        transpose = 1;
        swap(&x0, &y0);
        swap(&x1, &y1);
    }

    if (x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int x;
    int y;
    int dx = abs(x1 - x0);
    int de = 2 * abs(y1 - y0);
    int e = 0;
    for (x = x0, y = y0; x <= x1; ++x) {
        if (transpose) {
            tgaSetPixel(image, y, x, color);
        } else {
            tgaSetPixel(image, x, y, color);
        }

        e += de;
        if (e > dx) {
            y += sign(y1 - y0);
            e -= 2 * dx;
        }
    }
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int abs(int a) {
    return (a >= 0) ? a : -a;
}

int sign(int a) {
    if (a > 0) {
        return 1;
    } else if (a == 0) {
        return 0;
    } else {
        return -1;
    }
}

