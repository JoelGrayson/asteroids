#include "asteroid.h"

struct asteroid {
    int x;
    int y;
    int vx;
    int vy;
    int size;
    enum asteroid_type type;
    double rotation; //0 to 360 degrees
};

