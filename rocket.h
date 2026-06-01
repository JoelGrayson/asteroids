#ifndef ROCKET_H
#define ROCKET_H

#include "graphics/point.h"
#include "graphics/geometry.h"

#define ROCKET_SIDES_DRIFT_SPEED 25

extern struct mechanics rocket_mechanics;

// Original number of rocket points vs. number of rocket points we tell other files there are.
#define __ROCKET_NUM_POINTS 6
extern int ROCKET_NUM_POINTS;
extern int ROCKET_EXPLODED_SIDES_NUM_POINTS; // At the start, we don't want to draw the exploded rocket sides.

// Rocket points polygon
extern struct point ROCKET_POINTS[__ROCKET_NUM_POINTS];

// Rocket sides polygons for when we want to display them.
extern struct point ROCKET_EXPLODED_POINTS[__ROCKET_NUM_POINTS-1][2];

// Helper functions to intialize rocket exploded sides and then update their positions continuously.
void rocket_explode_init(void);
void rocket_explode_update(void);
void rocket_update_mechanics();





rotate_rocket_theta()
rotate_rocket_left() = rotate_rocket_theta(-1)
rotate_rocket_right() = rotate_rocket_theta(+1)
get_rocket_mechanics()
explode_rocket()
render_rocket()
update_rocket_mechanics()
rocket_fire()
rocket_thrust()


#endif
