#include "draw_asteroid.h"

void draw_asteroid(struct point asteroid[], color_t c) {
    for (int i = 0; i < NUM_POINTS_PER_ASTEROID - 1; i++) {
        draw_line(asteroid[i], asteroid[i + 1], c);
    }
}


struct point ASTEROID_1_POINTS[NUM_POINTS_PER_ASTEROID] = {
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

