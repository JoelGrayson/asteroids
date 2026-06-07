#ifndef ROCKET_H
#define ROCKET_H

#include "graphics/point.h"
#include "graphics/geometry.h"
#include "mechanics.h"
#include "constants.h"
#include "printf.h"
#include <stdbool.h>
#include "graphics/rotate_points.h"
#include "graphics/draw_points.h"
#include "rocket.h"
#include "explosion.h"
#include <stdbool.h>
#include "graphics/geometry.h"
#include "graphics/rotate_vector.h"
#include "maths.h"
#include "timer.h"
#include "rand.h"
#include "bullets.h"
#include "asteroid.h"
#include "audio/sounds.h"

#define ROCKET_NUM_POINTS 6
#define FIRE_SOUND_TICK_DURATION 8
#define THRUST_SOUND_TICK_DURATION 400000*TICKS_PER_USEC

extern struct point ROCKET_POINTS_TEMPLATE[ROCKET_NUM_POINTS];
extern struct point MINI_ROCKET_POINTS[ROCKET_NUM_POINTS];

extern struct point rotated_rocket_points[ROCKET_NUM_POINTS]; //exported for collision_detection

extern bool rocket_is_exploding;

void setup_rocket();


// The five arcade buttons
void rocket_rotate_left_press();
void rocket_rotate_left_release();
void rocket_rotate_right_press();
void rocket_rotate_right_release();
void rocket_fire();
void rocket_thrust();
void rocket_unthrust();
void rocket_hyperspace();

// Increases the rotation of the rocket by theta. Then, changes the internal rotated_rocket_points to ROCKET_POINTS_TEMPLATE rotated by an amount (so only recomputes rotation when the rotation changes)
void loop_rocket();
void rocket_explode();
struct mechanics get_rocket_mechanics();
struct point *get_points_of_rocket(); // returns rockets' points (as currently rendered in rotation)
int get_num_rocket_points(); // returns the total count of points forming rocket polygon
bool rocket_asteroid_collision();
bool rocket_is_invincible(); // returns whether the rocket is currently invincible or not.
bool is_rocket_exploding(); // returns whether the rocket is currently exploding or not.

#endif

