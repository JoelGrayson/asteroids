#include "gl.h"
#include <stdbool.h>
#include "maths.h"
#include "gl_lib.h"

void draw_line(struct point p1, struct point p2, color_t c) {
    // dx/dy is what you have to do to move from p1 to p2
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    // If y is the larger difference, move from p1.y to p2.y by ±1 and along the way have an appropriate x increase (by ±<1)
    axis direction_of_most_movement = abs(dy) > abs(dx) ? Y : X;

    // pos moves from point 1 to point 2
    struct point pos = p1;
    
    if (direction_of_most_movement == Y) {
        int sgn_dy = sgn(dy);
        // For every +1 in pos.y, pos.x increases by dx/dy
        double dx_div_dy = dx / dy;

        while (true) {
            gl_draw_pixel(round(pos.x), round(pos.y), c); //double rounded to int so it draws the nearest pixel

            pos.y += sgn_dy;
            pos.x += sgn_dy * dx_div_dy;

            // Check if done (drew past p2). If done, return
            if (
                // Increasing
                (sgn_dy == 1 && pos.y >= p2.y)
                ||
                // Decreasing
                (sgn_dy == -1 && pos.y <= p2.y)
            ) {
                return;
            }
        }
    } else {

    }
}

