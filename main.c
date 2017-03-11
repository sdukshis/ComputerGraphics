#include <stdio.h>
#include <math.h>

#include "tga.h"

void swap(int *a, int *b);
int abs(int a);

/*
* Using Digital Differential Analyzer algorihm
* to draw interval connecting (x0, y0) with (x1, y1)
* on image using color
*/
void line (tgaImage *image, 
           int x0, int y0,
           int x1, int y1,
           tgaColor color);

void line2 (tgaImage *image, 
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
    tgaImage * image = tgaNewImage(100, 100, RGB);

    int i;
    tgaColor white = tgaRGB(255, 255, 255);
    tgaColor red = tgaRGB(255, 0, 0);
    tgaColor blue = tgaRGB(0, 0, 255);
    for (i = 0; i < 1000000; ++i) {
        line2(image, 13, 20, 90, 40, white);
        line2(image, 20, 13, 40, 80, red);
        line2(image, 80, 40, 13, 20, blue);
    }
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
    int steep = 0;
    if (abs(y1 - y0) > abs(x1 - x0)) {
        steep = 1;
        swap(&x0, &y0);
        swap(&x1, &y1);
    }

    if (x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int x;
    double y;
    double k = ((double)(y1 - y0))/(x1 - x0);
    for (x = x0, y = y0; x <= x1; ++x, y += k) {
        if (steep) {
            tgaSetPixel(image, y, x, color);
        } else {
            tgaSetPixel(image, x, y, color);
        }
    }
}

void line2(tgaImage *image, 
           int x0, int y0,
           int x1, int y1,
           tgaColor color)
{
    int steep = 0;
    if (abs(y1 - y0) > abs(x1 - x0)) {
        steep = 1;
        swap(&x0, &y0);
        swap(&x1, &y1);
    }

    if (x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    int derror2 = 2*abs(dy);
    int error2 = 0;
    int sign = dy > 0 ? 1 : -1;
    int x;
    int y;
    for (x = x0, y = y0; x <= x1; ++x) {
        if (steep) {
            tgaSetPixel(image, y, x, color);
        } else {
            tgaSetPixel(image, x, y, color);
        }

        error2 += derror2;

        if (error2 > dx) {
            y += sign;
            error2 -= 2*dx; 
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
