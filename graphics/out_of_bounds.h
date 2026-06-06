#pragma once

#include <stdbool.h>
#include "../constants.h"
#include "point.h"

/* If an object of a given width and height is out of bounds */
bool out_of_bounds(struct point p, double width, double height) {
    return (
        (p.x - width / 2) < 0
        || (p.x + width / 2) > MONITOR_WIDTH
        || (p.y - height / 2) < 0
        || (p.y + height / 2) > MONITOR_HEIGHT
    );
}

