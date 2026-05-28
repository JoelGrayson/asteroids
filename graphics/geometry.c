#include "geometry.h"
#include "../maths.h"

struct point midpoint(struct point p1, struct point p2) {
    struct point mid = {
        .x = (p1.x + p2.x)/2,
        .y = (p1.y + p2.y)/2
    };
    return mid;
}
struct point point_plus_vec(struct point fulcrum, struct vector dir) {
    struct point new_location = {
        .x = fulcrum.x + dir.x,
        .y = fulcrum.y + dir.y
    };
    return new_location;
}
struct vector vec_difference(struct point p1, struct point p2) {
    struct vector diff = {
        .x = p1.x - p2.x, 
        .y = p1.y - p2.y
    };
    return diff;
}
struct vector vec_normalize(struct vector vec) {
    double normalization_factor = max(vec.x, vec.y);
    vec.x /= normalization_factor;
    vec.y /= normalization_factor;
    return vec;
}
struct vector vec_orthogonal(struct vector vec) {
    struct vector ortho = {
        .x = vec.y,
        .y = (-1)*vec.x 
    };
    return ortho;
}
