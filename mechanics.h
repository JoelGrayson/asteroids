#ifndef MECHANICS_H
#define MECHANICS_H

#include "constants.h"
#include <stdbool.h>

struct mechanics {
    double x;
    double y;
    double vx;
    double vy;
    double rotation; //heading/bearing from North. pi/2 is East
};

/* Base mechanics updater (updates x and y according to vx, vy, assumes inertia).
* @param wrap_around if true (like for rocket and saucer), spawns at the left side if gone off the right side
*/
void update_mechanics(struct mechanics* mech, bool wrap_around);

#endif

