#include "mechanics.h"

void update_mechanics(struct mechanics* mech, bool wrap_around) {
    // Velocity increments by acceleration
    mech->vx += mech->ax;
    mech->vy += mech->ay;
    // Position increments by velocity
    mech->x += mech->vx;
    mech->y += mech->vy;

    // Bounds checking
    if (wrap_around) {
        if (mech->x < 0) {
            mech->x = MONITOR_WIDTH;
        }
        if (mech->x > MONITOR_WIDTH) {
            mech->x = 0;
        }
        
        if (mech->y < 0) {
            mech->y = MONITOR_HEIGHT;
        }
        if (mech->y > MONITOR_HEIGHT) {
            mech->y = 0;
        }
    }
}

