#include "saucer.h"

struct point SAUCER_EXTERIOR_POINTS[] = {
    { 25, 16 },
    { 35, 16 },
    { 39, 25 },
    { 52, 35 },
    { 41, 45 },
    { 21, 45 },
    { 10, 35 },
    { 21, 25 },
    { 25, 16 }
};
#define NUM_SAUCER_EXTERIOR_POINTS (sizeof(SAUCER_EXTERIOR_POINTS) / sizeof(SAUCER_EXTERIOR_POINTS[0]))
struct point SAUCER_TOP_LINE_POINTS[] = {
    { 21, 25 },
    { 39, 25 }
};
struct point SAUCER_BOTTOM_LINE_POINTS[] = {
    { 10, 35 },
    { 52, 35 }
};

void render_saucer() {
    draw_points(SAUCER_EXTERIOR_POINTS, NUM_SAUCER_EXTERIOR_POINTS, 100, 100, GL_WHITE);
}

