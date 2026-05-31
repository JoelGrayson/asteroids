#include "mechanics.h"

void update_mechanics(struct mechanics* mech) {
    // Velocity increments by acceleration.
    mech->vx += mech->ax;
    mech->vy += mech->ay;

    // Position increments by velocity.
    mech->x += mech->vx;
    mech->y += mech->vy;
}