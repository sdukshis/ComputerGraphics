#include <stdio.h>
#include <stdlib.h>

#include "tga.h"
#include "model.h"

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


void triangle(tgaImage *image,
              int x0, int y0,
              int x1, int y1,
              int x2, int y2,
              tgaColor color);
/*
* Draw model using meshgrid
*/
void meshgrid(tgaImage *image, Model *model);

void render(tgaImage *image, Model *model);

int main(int argc, char **argv)
{
    int rv = 0;
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <objfile> <outfile>\n", argv[0]);
        return -1;
    }

    Model *model = NULL;
    tgaImage *image = NULL;

    do {
        model = loadFromObj(argv[1]);
        if (!model) {
            perror("loadFromObj");
            rv = -1;
            break;
        }

        image = tgaNewImage(800, 800, RGB);
        if (!image) {
            perror("tgaNewImage");
            rv = -1;
            break;
        }

        // triangle(image, 0, 0, 100, 100, 20, 200, tgaRGB(255, 255, 255));
        // meshgrid(image, model);
        render(image, model);

        if (-1 == tgaSaveToFile(image, argv[2])) {
            perror("tgaSateToFile");
            rv = -1;
            break;
        }
    } while (0);

    if (model) {
        freeModel(model);
    }
    if (image) {
        tgaFreeImage(image);
    }  
    return rv;
}

void meshgrid(tgaImage *image, Model *model)
{
    int face, vert, i;
    int h = image->height;
    int w = image->width;
    tgaColor white = tgaRGB(255, 255, 255);
    Vec3 *p[3];
    for (face = 0; face < model->nface; ++face) {
        for (vert = 0; vert < 3; ++vert) {
            p[vert] = getVertex(model, face, vert);
        }
        for (i = 0; i < 3; ++i) {
            line(image,
                ((*p[i])[0] + 1.0)*w/2,         ((*p[i])[1] + 1.0)*h/2,
                ((*p[(i + 1)%3])[0] + 1.0)*w/2, ((*p[(i + 1)%3])[1] + 1.0)*h/2,
                white);
        }
    }
    tgaFlipVertically(image);
}

void render(tgaImage *image, Model *model)
{
    int face, vert;
    int h = image->height;
    int w = image->width;
    Vec3 *p[3];
    int screen_coords[3][2];
    for (face = 0; face < model->nface; ++face) {
        for (vert = 0; vert < 3; ++vert) {
            p[vert] = getVertex(model, face, vert);
            screen_coords[vert][0] = ((*p[vert])[0] + 1.0)*w/2;
            screen_coords[vert][1] = ((*p[vert])[1] + 1.0)*h/2;
        }
        triangle(image, screen_coords[0][0], screen_coords[0][1],
                        screen_coords[1][0], screen_coords[1][1],
                        screen_coords[2][0], screen_coords[2][1],
                        tgaRGB(rand()%256, rand()%256, rand()%256));
    }
    tgaFlipVertically(image);    
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

void triangle(tgaImage *image,
              int x0, int y0,
              int x1, int y1,
              int x2, int y2,
              tgaColor color)
{
    /* Sort vertices by y coord */
    if (y0 > y1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    if (y0 > y2) {
        swap(&x0, &x2);
        swap(&y0, &y2);
    }
    if (y1 > y2) {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    /* Compute x coord deltas */
    double dx01 = 0, dx02 = 0, dx12 = 0;
    if (y0 != y1) {
        dx01 = x1 - x0;
        dx01 /= y1 - y0;
    }
    if (y0 != y2) {
        dx02 = x2 - x0;
        dx02 /= y2 - y0;
    }
    if (y1 != y2) {
        dx12 = x2 - x1;
        dx12 /= y2 - y1;
    }
    double _dx02 = dx02;

    if (dx01 > dx02) {
        double t = dx01;
        dx01 = dx02;
        dx02 = t;
    }

    if (dx12 > _dx02) {
        double t = dx12;
        dx12 = _dx02;
        _dx02 = t;
    }

    /* Fill the triangle */
    int y, x;
    double xleft = x0, xright = x0;
    for (y = y0; y <= y2; ++y) {
        for (x = xleft; x <= xright; ++x) {
            tgaSetPixel(image, x, y, color);
        }
        xleft += (y <= y1) ? dx01 : _dx02;
        xright += (y <= y1) ? dx02 : dx12;
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
