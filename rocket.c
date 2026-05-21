#include "rocket.h"
#include "mechanics.h"
#include "constants.h"

static struct mechanics rocket_mechanics = {
    .x = MONITOR_WIDTH / 2,
    .y = MONITOR_HEIGHT / 2,
    .vx = 0,
    .vy = 0,
    .ax = 0,
    .ay = 0
};

