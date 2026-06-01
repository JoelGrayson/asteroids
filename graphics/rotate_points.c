#include "malloc.h"
#include "../maths.h"

// Rotates a list of points around x,y = 0
struct point *rotate_points(struct point *points, int num_points, double theta) {
    struct point *rotated = malloc(num_points*sizeof(struct point));
    for(int i = 0; i < num_points; i++) {
        struct to_rotate = points[i];
        rotated.x = (cosine(theta) * to_rotate.x) + (sine(theta) * to_rotate.y);
        rotated.y = (-1*sine(theta) * to_rotate.x) + (cos(theta) * to_rotate.y);
    }
    return rotated;
}

