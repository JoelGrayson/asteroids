#include "malloc.h"
#include "point.h"
#include "../maths.h"

// Rotates a list of points around x,y = 0
struct point *rotate_points(struct point *points, int num_points, double theta) {
    struct point *rotated = malloc(num_points*sizeof(struct point));
    for(int i = 0; i < num_points; i++) {
        struct point to_rotate = points[i];
        rotated[i].x = (cosine(theta) * to_rotate.x) + (sine(theta) * to_rotate.y);
        rotated[i].y = (-1*sine(theta) * to_rotate.x) + (cosine(theta) * to_rotate.y);
    }
    return rotated;
}

