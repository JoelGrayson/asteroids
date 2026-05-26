#include "draw_points.h"

/** draws a list of points (length num_points) offset by (x_offset, y_offset) */
void draw_points(struct point *points, int num_points, int x_offset, int y_offset, color_t c) {
    for (int i = 0; i < num_points - 1; i++) {
        draw_line(points[i], points[i + 1], c);
    }
}

