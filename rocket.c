#include "rocket.h"
#include "mechanics.h"
#include "constants.h"

struct point ROCKET[ROCKET_NUM_POINTS] = {
    { 25, 5 },
    { 12, 45 },
    { 15, 36 },
    { 35, 36 },
    { 38, 45 }
};


static struct mechanics rocket_mechanics = {
    .x = MONITOR_WIDTH / 2,
    .y = MONITOR_HEIGHT / 2,
    .vx = 0,
    .vy = 0,
    .ax = 0,
    .ay = 0,
    .rotation = 0
};

void draw_rocket() {
    
}

