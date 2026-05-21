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

#endif

