#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "tga.h"

void die(const char* fmt, ...) {
    va_list args;
    va_start (args, fmt);
    vfprintf (stderr, fmt, args);
    va_end (args);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        die("Usage: %s <tgafile> width height\n", argv[0]);
    }
    char *tgafile = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    tgaImage* image = tgaLoadFromFile(tgafile);
    if (!image) {
        die("loadFromObj failed: %s\n", strerror(errno));
    }

    if (image->width != width) {
        die("width doen't match\n");
    }
    if (image->height != height) {
        die("height doen't match\n");
    }
    return 0;
}