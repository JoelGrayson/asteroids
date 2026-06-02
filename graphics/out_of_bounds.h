#pragma once

#include <stdbool.h>
#include "../constants.h"
#include "point.h"

bool out_of_bounds(struct point p) {
    return (
        p.x < 0
        || p.x > MONITOR_WIDTH
        || p.y < 0
        || p.y > MONITOR_HEIGHT
    );
}

