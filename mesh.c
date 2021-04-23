#include <stdio.h>
#include "tga.h"
#include "model.h"

#include <assert.h>

void swap(int *a, int *b);
int iabs(int a);

/*
* Using Digital Differential Analyzer algorihm
* to draw interval connecting (x0, y0) with (x1, y1)
* on image using color
*/
void line (int x0, int y0,
           int x1, int y1,
           tgaImage *image,
           tgaColor color);

void meshgrid(tgaImage *image, Model *model);

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <objfile> <outfile>\n", argv[0]);
        return -1;
    }

    Model *model = loadFromObj(argv[1]);
    if (!model) {
        perror("loadFromObj");
        return -1;
    }
    printf("model with %d vertices, %d faces loaded\n", model->nvert, model->nface);
    int rv = 0;

    int height = 800;
    int width = 800;

    tgaImage * image = tgaNewImage(height, width, RGB);

    meshgrid(image, model);

    if (-1 == tgaSaveToFile(image, argv[2])) {
        perror("tgaSateToFile");
        rv = -1;
    }
    tgaFreeImage(image);
    freeModel(model);

    return rv;
}

void meshgrid(tgaImage *image, Model *model)
{
    assert(image);
    assert(model);
    int i;
    int h = image->height;
    int w = image->width;
    for (i = 0; i < model->nface; ++i) {
        Vec3 *v0 = &(model->vertices[model->faces[i][0]]);
        Vec3 *v1 = &(model->vertices[model->faces[i][3]]);
        Vec3 *v2 = &(model->vertices[model->faces[i][6]]);

        line(((*v0)[0] + 1) * w / 2,(1 - (*v0)[1]) * h / 2,
             ((*v1)[0] + 1) * w / 2,(1 - (*v1)[1]) * h / 2,
             image, tgaRGB(255, 255, 255));

        line(((*v1)[0] + 1) * w / 2,(1 - (*v1)[1]) * h / 2,
             ((*v2)[0] + 1) * w / 2,(1 - (*v2)[1]) * h / 2,
             image, tgaRGB(255, 255, 255));

        line(((*v2)[0] + 1) * w / 2,(1 - (*v2)[1]) * h / 2,
             ((*v0)[0] + 1) * w / 2,(1 - (*v0)[1]) * h / 2,
             image, tgaRGB(255, 255, 255));
    }
}

void line (int x0, int y0,
           int x1, int y1,
           tgaImage *image,
           tgaColor color)
{
    int steep = 0;
    if (iabs(y1 - y0) > iabs(x1 - x0)) {
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
            tgaSetPixel(image, (unsigned int)y, (unsigned int)x, color);
        } else {
            tgaSetPixel(image, (unsigned int)x, (unsigned int)y, color);
        }
    }
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int iabs(int a) {
    return (a >= 0) ? a : -a;
}
