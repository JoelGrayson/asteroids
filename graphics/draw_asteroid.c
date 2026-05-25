#include "draw_asteroid.h"

void draw_asteroid(struct point asteroid[], color_t c) {
    for (int i = 0; i < NUM_POINTS_PER_ASTEROID - 1; i++) {
        draw_line(asteroid[i], asteroid[i + 1], c);
    }
}

