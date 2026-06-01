#ifndef ASTEROID_H
#define ASTEROID_H

#include "mechanics.h"
#include "graphics/point.h"
#include "printf.h"
#include <stdbool.h>

#define MAX_NUM_ASTEROIDS 1000
extern unsigned int MAX_ASTEROID_SPEED;

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
    bool is_exploding; //false by default
    int exploding_frame; //starts at 0
};

#define ASTEROID_NUM_POINTS 13
// Extern means not a declaration but rather telling compiler that it is there somewhere and linker will find it
extern struct point ASTEROID_A_SMALL_POINTS[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_B_SMALL_POINTS[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_C_SMALL_POINTS[ASTEROID_NUM_POINTS];

extern struct point ASTEROID_A_MEDIUM_POINTS[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_B_MEDIUM_POINTS[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_C_MEDIUM_POINTS[ASTEROID_NUM_POINTS];

extern struct point ASTEROID_A_BIG_POINTS[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_B_BIG_POINTS[ASTEROID_NUM_POINTS];
extern struct point ASTEROID_C_BIG_POINTS[ASTEROID_NUM_POINTS];

struct point *get_points_of_asteroid(struct asteroid ast);
void asteroids_update_mechanics(struct asteroid* asteroids, int num_asteroids);
struct point asteroid_get_pos(struct asteroid ast);
void asteroid_set_pos(struct asteroid* ast, struct point new_pos);
void asteroid_respawn(struct asteroid* ast);

#endif

