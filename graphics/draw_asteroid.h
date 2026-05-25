#ifndef ASTEROID_H
#define ASTEROID_H

#include "gl.h"
#include "draw_line.h"

void draw_asteroid(struct point asteroid[], color_t c);

#define NUM_POINTS_PER_ASTEROID 13
extern struct point ASTEROID_1_POINTS[NUM_POINTS_PER_ASTEROID]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it

#endif
