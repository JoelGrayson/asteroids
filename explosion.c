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

    // Centered to zero
    { -13.9, -21.8 },
    {   9.1, -21.8 },
    {  -2.9, -15.8 },
    {  12.1,  -6.8 },
    {  -6.9,  -0.8 },
    { -13.9,   8.2 },
    {  10.1,   8.2 },
    {   3.1,  13.2 },
    {  -6.9,  18.2 },
    {  10.1,  19.2 },
};

void render_explosion(struct point position, int explosion_frame_number) {
    if (explosion_frame_number > NUM_FRAMES_OF_EXPLOSION) {
        return; //nothing to draw
    }

    double ratio_complete = (double)explosion_frame_number / (double)NUM_FRAMES_OF_EXPLOSION;

    for (int i = 0; i < NUM_EXPLOSION_POINTS; i++) {
        gl_draw_pixel(
            explosion_points[i].x * ratio_complete + position.x,
            explosion_points[i].y * ratio_complete + position.y,
            GL_WHITE
        );
    }
}

