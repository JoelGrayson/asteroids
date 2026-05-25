#ifndef ASTEROID_H
#define ASTEROID_H

#include "mechanics.h"

#define MAX_NUM_ASTEROIDS 1000

enum asteroid_type {
    A,
    B,
    C
};

enum asteroid_size {
    BIG,
    MEDIUM,
    SMALL
};

struct asteroid {
    enum asteroid_type type;
    enum asteroid_size size;
    struct mechanics mechanics;
};

#endif

