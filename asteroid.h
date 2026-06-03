#ifndef ASTEROID_H
#define ASTEROID_H

#include "mechanics.h"
#include "graphics/point.h"
#include "printf.h"
#include <stdbool.h>

#define MAX_NUM_ASTEROIDS 1000
extern unsigned int MAX_ASTEROID_SPEED;

#define EXPLOSION_SOUND_TICK_DURATION 1500000*TICKS_PER_USEC

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
void asteroids_update_mechanics();
struct point asteroid_get_pos(struct asteroid ast);
void asteroid_set_pos(struct asteroid* ast, struct point new_pos);
void asteroid_spawn();
void asteroid_despawn(struct asteroid_list_t* ast);
int get_num_asteroid_points(); // returns the total count of points forming asteroid polygon
struct asteroid_list_t *get_next_spawnable_asteroid(); // Finds the pointer of the next asteroid which can be allocated out of the list of asteroids.
void render_asteroids(); // draws asteroid list
// If any of the allocated asteroids collides with said object, return a pointer to that asteroid; otherwise return NULL.
struct asteroid_list_t *get_asteroid_collision(struct mechanics obj, struct point *points_obj, int num_points_obj);
void asteroid_explode(struct asteroid_list_t *ast); // Explodes a particular asteroid (with particle effects! and deallocates it).
#endif

