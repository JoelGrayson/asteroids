#pragma once

#include "mechanics.h"
#include "printf.h"
#include "gl.h"
#include "malloc.h"
#include "graphics/out_of_bounds.h"

/* Represents who shot the bullet */
#define MAX_NUM_BULLETS 40

enum bullet_owner {
    ROCKET,
    SAUCER
};

// This is not a template. No rotation rendering on bullet
#define BULLET_NUM_POINTS 4


/* Represents a bullet */
struct bullet {
    enum bullet_owner owner;
    struct mechanics mechanics;
};

/* Creates a new bullet with specified mechanics and owner */
void new_bullet(struct mechanics mechanics, enum bullet_owner owner);

// Extern of list of pointers to bullets. Each element of list is either NULL (no bullet) or a pointer to a bullet
// Use extern pattern instead of get_bullets because then it would copy
extern struct bullet *bullets[MAX_NUM_BULLETS];

void draw_bullet(struct bullet *bullet) ;

void setup_bullets();
void loop_bullets();
int get_curr_num_bullets();

struct point *get_bullet_points();

// Collision handling function for when bullet collides with an asteroid.
void bullets_asteroid_collision();

// Deallocates a bullet from our list on command, given a pointer to it.
void delete_bullet(struct bullet **bullet);

extern struct point BULLET_POINTS[BULLET_NUM_POINTS];
extern struct point BULLET_COLLISION_POINTS[BULLET_NUM_POINTS];

