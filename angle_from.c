#include "angle_from.h"

double angle_from(struct point p1, struct point p2) {
    double dy = p2.y - p1.y;
    double dx = p2.x - p1.x;

    // -dy because the y-axis is flipped in our implementation
    // -π/2 because using degrees from facing north not east
    return atan2(-dy, dx) - PI / 2;
}

