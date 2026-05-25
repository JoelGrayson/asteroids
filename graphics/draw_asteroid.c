#include "draw_asteroid.h"

void draw_asteroid(struct point asteroid[], color_t c) {
    for (int i = 0; i < ASTEROID_A_NUM_POINTS - 1; i++) {
        draw_line(asteroid[i], asteroid[i + 1], c);
    }
}


struct point ASTEROID_A_SMALL[ASTEROID_A_NUM_POINTS] = {
    { 10, 5 },
    { 19, 5 },
    { 27, 9 },
    { 27, 12 },
    { 19, 14 },
    { 27, 19 },
    { 21, 24 },
    { 18, 21 },
    { 10, 24 },
    { 4, 17 },
    { 4, 9 },
    { 13, 9 },
    { 10, 5 }
};

struct point ASTEROID_B_SMALL[ASTEROID_B_NUM_POINTS] = {
    { 10, 5 },
    { 15, 10 },
    { 21, 5 },
    { 26, 10 },
    { 23, 15 },
    { 26, 20 },
    { 18, 25 },
    { 10, 25 },
    { 4, 20 },
    { 4, 17 },
    { 4, 10 },
};

struct point ASTEROID_C_SMALL[ASTEROID_C_NUM_POINTS] = {
    { 10, 5 },
    { 4, 10 },
    { 7, 15 },
    { 4, 19 },
    { 9, 24 },
    { 13, 21 },
    { 21, 24 },
    { 26, 17 },
    { 21, 13 },
    { 26, 9 },
    { 21, 5 },
    { 15, 7 },
};

