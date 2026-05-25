#include "draw_points.h"

void draw_points(struct point *points, int num_points, color_t c) {
    for (int i = 0; i < num_points - 1; i++) {
        draw_line(points[i], points[i + 1], c);
    }
}

