#ifndef ASTEROID_H
#define ASTEROID_H

#include "gl.h"
#include "draw_line.h"

struct point {
    double x;
    double y;
};

void draw_line(struct point p1, struct point p2, color_t c);

void draw_asteroid();

#endif
