#include "explosion.h"

#define NUM_EXPLOSION_POINTS 10
struct point explosion_points[NUM_EXPLOSION_POINTS] = {
    // { 18, 8 },
    // { 41, 8 },
    // { 29, 14 },
    // { 44, 23 },
    // { 25, 29 },
    // { 18, 38 },
    // { 42, 38 },
    // { 35, 43 },
    // { 25, 48 },
    // { 42, 49 },

    { 18, 8 },
    { 41, 8 },
    { 29, 14 },
    { 44, 23 },
    { 25, 29 },
    { 18, 38 },
    { 42, 38 },
    { 35, 43 },
    { 25, 48 },
    { 42, 49 },
};

void render_explosion(struct point position, int frame_number) {
    if (frame_number > NUM_FRAMES_OF_EXPLOSION) {
        return; //nothing to draw
    }

    double ratio_complete = frame_number / NUM_FRAMES_OF_EXPLOSION;

    for (int i = 0; i < NUM_EXPLOSION_POINTS; i++) {
        gl_draw_p
        explosion_points[i].x
    }
}

