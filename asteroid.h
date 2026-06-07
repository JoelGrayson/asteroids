#pragma once

#include "mechanics.h"
#include "graphics/point.h"
#include "printf.h"
#include <stdbool.h>
#include "explosion.h"
#include "mechanics.h"
#include "constants.h"
#include "graphics/point.h"
#include "graphics/geometry.h"
#include "graphics/draw_points.h"
#include "rand.h"
#include "timer.h"
#include "printf.h"
#include "audio/sounds.h"
#include "rocket.h"
#include "score_and_lives.h"
#include "frame.h"
#include "graphics/rotate_vector.h"

#define MAX_NUM_ASTEROIDS 1000
extern unsigned int MAX_ASTEROID_SPEED;

#define EXPLOSION_SOUND_TICK_DURATION 2000000*TICKS_PER_USEC

extern const int STARTING_NUM_FRAMES_BETWEEN_SPAWN;

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

struct asteroid_list_t {
    struct asteroid ast;
    bool allocated;
    bool allocate_on_next_frame; //whether it was created on the current frame and is yet to be allocated. This way, the new spawned asteroids don't immediately consume bullets.
};


void setup_asteroids();

// Draws asteroid list and calculates if you need to spawn any new asteroids
void loop_asteroids(); 

// If any of the allocated asteroids collides with said object, return a pointer to that asteroid; otherwise return NULL.
struct asteroid_list_t *get_asteroid_collision(struct mechanics obj, struct point *points_obj, int num_points_obj);
void asteroid_explode(struct asteroid_list_t *ast); // Explodes a particular asteroid (with particle effects! and deallocates it).
void asteroid_increase_score_by(enum asteroid_size asteroid_size);

extern struct asteroid_list_t asteroid_list[MAX_NUM_ASTEROIDS];

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

