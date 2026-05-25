#ifndef ASTEROID_H
#define ASTEROID_H

#include "mechanics.h"
#include "graphics/point.h"

#define MAX_NUM_ASTEROIDS 1000

enum asteroid_type {
    A,
    B,
    C
};

enum asteroid_size {
    BIG,
    MEDIUM,
    SMALL
};

struct asteroid {
    enum asteroid_type type;
    enum asteroid_size size;
    struct mechanics mechanics;
};


#define ASTEROID_NUM_POINTS 13
// Extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it
extern struct point ASTEROID_A_SMALL[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_B_SMALL[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_C_SMALL[ASTEROID_NUM_POINTS];

extern struct point ASTEROID_A_MEDIUM[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_B_MEDIUM[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_C_MEDIUM[ASTEROID_NUM_POINTS];

extern struct point ASTEROID_A_BIG[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_B_BIG[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_C_BIG[ASTEROID_NUM_POINTS];

struct point *get_points_of_asteroid(struct asteroid ast);


#endif

