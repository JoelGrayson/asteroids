#ifndef ASTEROID_H
#define ASTEROID_H

#include "gl.h"
#include "draw_line.h"

void draw_asteroid(struct point asteroid[], color_t c);

const int NUM_POINTS_PER_ASTEROID = 13;
struct point ASTEROID_1_POINTS[] = {
    { 10, 5 },
    { 19, 5 },
    { 27, 9 },
    { 27, 12 },
    { 19, 14 },
    { 27, 19 },
    { 21, 24 },
    { 18, 21 },
    { 10, 24 },
    { 4, 17 },
    { 4, 9 },
    { 13, 9 },
    { 10, 5 }
};
struct point ASTEROID_2_POINTS[] = {
    { 10, 5 },
    { 19, 5 },
    { 27, 9 },
    { 27, 12 },
    { 19, 14 },
    { 27, 19 },
    { 21, 24 },
    { 18, 21 },
    { 10, 24 },
    { 4, 17 },
    { 4, 9 },
    { 13, 9 },
    { 10, 5 }
};
struct point ASTEROID_3_POINTS[] = {
    { 10, 5 },
    { 19, 5 },
    { 27, 9 },
    { 27, 12 },
    { 19, 14 },
    { 27, 19 },
    { 21, 24 },
    { 18, 21 },
    { 10, 24 },
    { 4, 17 },
    { 4, 9 },
    { 13, 9 },
    { 10, 5 }
};


#endif
