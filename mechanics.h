#ifndef MECHANICS_H
#define MECHANICS_H

struct mechanics {
    double x;
    double y;
    double vx;
    double vy;
    double ax; //acceleration
    double ay;
    double rotation; //heading/bearing from North. pi/2 is East
};

// Base mechanics updater (updates x and y according to vx, vy, updates vx, vy according to ax, ay; assumes inertia).
void update_mechanics(struct mechanics* mech);

#endif

