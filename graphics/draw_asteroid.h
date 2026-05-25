#ifndef DRAW_POINTS_H
#define DRAW_POINTS_H

#include "gl.h"
#include "draw_line.h"
#include "../asteroid.h"

void draw_points(struct point *points, color_t c);

#define ASTEROID_NUM_POINTS 13
extern struct point ASTEROID_A_SMALL[ASTEROID_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it
extern struct point ASTEROID_B_SMALL[ASTEROID_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it
extern struct point ASTEROID_C_SMALL[ASTEROID_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it

extern struct point ASTEROID_A_MEDIUM[ASTEROID_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it
extern struct point ASTEROID_B_MEDIUM[ASTEROID_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it
extern struct point ASTEROID_C_MEDIUM[ASTEROID_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it

extern struct point ASTEROID_A_LARGE[ASTEROID_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it
extern struct point ASTEROID_B_LARGE[ASTEROID_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it
extern struct point ASTEROID_C_LARGE[ASTEROID_NUM_POINTS]; //extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it

struct point *get_points(struct asteroid ast);

#endif
