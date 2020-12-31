#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "model.h"

void die(const char* fmt, ...) {
    va_list args;
    va_start (args, fmt);
    vfprintf (stderr, fmt, args);
    va_end (args);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        die("Usage: %s <objfile> nface nvert\n", argv[0]);
    }
    char *objfile = argv[1];
    int nface = atoi(argv[2]);
    int nvert = atoi(argv[3]);

    Model* model = loadFromObj(objfile);
    if (!model) {
        die("loadFromObj failed: %s\n", strerror(errno));
    }

    if (model->nface != nface) {
        die("face number doen't match\n");
    }
    if (model->nvert != nvert) {
        die("face number doen't match\n");
    }
    return 0;
}