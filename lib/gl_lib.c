#include "gl.h"
#include <stdbool.h>

typedef enum {
    X,
    Y
} axis;


struct point {
    double x;
    double y;
};

double abs(double num) {
    if (num < 0) {
        return -num;
    }
    return num;
}

double max(double a, double b) {
    if (a > b) {
        return a;
    }
    return b;
}

double sgn(double num) {
    if (num > 0) {
        return 1;
    }
    if (num < 0) {
        return -1;
    }
    return 0;
}

int round(double num) {
    // if num's tenth's place (1.t) has t<=4 then return (int)num else return (int)num+1
    // num*10 is xxxt.xxx
    // (int)(num*10) is xxxt
    // (int)(num*10) % 10 is t

    int t = (int)(num * 10) % 10;

    if (t <= 4) {
        return (int)num;
    }
    // else t >= 5
    return (int)num + 1;
}

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

