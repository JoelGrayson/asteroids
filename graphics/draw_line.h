#ifndef DRAW_LINE_H
#define DRAW_LINE_H

#include "gl.h"
#include "point.h"
#include <stdbool.h>
#include "../maths.h"

typedef enum {
    X,
    Y
} axis;

void draw_line(struct point p1, struct point p2, color_t c);

#endif
