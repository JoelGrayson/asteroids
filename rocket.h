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


#define ROCKET_NUM_POINTS 6
#define FIRE_SOUND_TICK_DURATION 400000*TICKS_PER_USEC
#define THRUST_SOUND_TICK_DURATION 400000*TICKS_PER_USEC

extern struct point ROCKET_POINTS_TEMPLATE[ROCKET_NUM_POINTS];

void reset_rocket();


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
void loop_rocket(long frame);
void rocket_explode();
struct mechanics get_rocket_mechanics();
void rocket_update_mechanics(); //called once every cycle to update the position based on speed
struct point *get_points_of_rocket(); // returns rockets' points (as currently rendered in rotation)
int get_num_rocket_points(); // returns the total count of points forming rocket polygon
bool rocket_asteroid_collision();

#endif

