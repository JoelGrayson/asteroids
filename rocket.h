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


#define ROCKET_NUM_POINTS 6




// #define ROCKET_SIDES_DRIFT_SPEED 25

// extern struct mechanics rocket_mechanics;

// // Original number of rocket points vs. number of rocket points we tell other files there are.
// #define __ROCKET_NUM_POINTS 6
// extern int ROCKET_NUM_POINTS;
// extern int ROCKET_EXPLODED_SIDES_NUM_POINTS; // At the start, we don't want to draw the exploded rocket sides.

// // Rocket points polygon
// extern struct point ROCKET_POINTS_TEMPLATE[__ROCKET_NUM_POINTS];

// // Rocket sides polygons for when we want to display them.
// extern struct point ROCKET_EXPLODED_POINTS[__ROCKET_NUM_POINTS-1][2];

// // Helper functions to intialize rocket exploded sides and then update their positions continuously.
// void rocket_explode_init(void);
// void rocket_explode_update(void);
// void rocket_update_mechanics();






// New interface

// The five arcade buttons
void rocket_rotate_left(); // = rotate_rocket_theta(-1)
void rocket_rotate_right(); // = rotate_rocket_theta(+1)
void rocket_fire();
void rocket_thrust();
void rocket_unthrust();
void rocket_hyperspace();

// Increases the rotation of the rocket by theta. Then, changes the internal rotated_rocket_points to ROCKET_POINTS_TEMPLATE rotated by an amount (so only recomputes rotation when the rotation changes)
void rocket_rotate_radians(double theta);
void render_rocket(); //just draws the rocket
void explode_rocket();
struct mechanics get_rocket_mechanics();
void rocket_update_mechanics(); //called once every cycle to update the position based on speed
struct point *get_points_of_rocket(); // returns rockets' points (as currently rendered in rotation)
int get_num_rocket_points(); // returns the total count of points forming rocket polygon

#endif
