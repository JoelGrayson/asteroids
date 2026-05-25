#ifndef ASTEROID_H
#define ASTEROID_H

#include "gl.h"
#include "draw_line.h"

void draw_asteroid(struct point asteroid[], color_t c);

#define ASTEROID_A_NUM_POINTS 13
#define ASTEROID_B_NUM_POINTS 11
#define ASTEROID_C_NUM_POINTS 12
extern struct point ASTEROID_A_SMALL[ASTEROID_A_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it
extern struct point ASTEROID_B_SMALL[ASTEROID_B_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it
extern struct point ASTEROID_C_SMALL[ASTEROID_C_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it

#endif
