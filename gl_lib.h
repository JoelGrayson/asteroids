#ifndef GL_LIB_H
#define GL_LIB_H

#include "gl.h"

typedef enum {
    X,
    Y
} axis;


struct point {
    double x;
    double y;
};

void draw_line(struct point p1, struct point p2, color_t c);

#endif
