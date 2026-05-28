#include "rocket.h"
#include "mechanics.h"
#include "constants.h"

int ROCKET_NUM_POINTS = __ROCKET_NUM_POINTS;
int ROCKET_EXPLODED_SIDES_NUM_POINTS = 0;

struct vector ROCKET_SIDES_DRIFT_VECS[__ROCKET_NUM_POINTS];

// Rocket points, polygon.
struct point ROCKET_POINTS[__ROCKET_NUM_POINTS] = {
    { 25, 5 },
    { 12, 45 },
    { 15, 36 },
    { 35, 36 },
    { 38, 45 },
    { 25, 5 },
};

// Array of rocket sides polygons.
struct point ROCKET_EXPLODED_POINTS[__ROCKET_NUM_POINTS][2] = {
    {   {25,5}, {12,45}     },
    {   {12,45}, {15,36}    },
    {   {15,36}, {35,36}    },
    {   {35,36}, {38,45}    },
    {   {38,45}, {25,5}     }
};

static struct mechanics rocket_mechanics = {
    .x = MONITOR_WIDTH / 2,
    .y = MONITOR_HEIGHT / 2,
    .vx = 0,
    .vy = 0,
    .ax = 0,
    .ay = 0,
    .rotation = 0
};

void rocket_explode_init() {
    // Copies present positions of rocket sides to exploded sides arrays in order to isolate the sides
    // each in their own polygon array able to fly off in different directions.
    for(int i = 0; i < __ROCKET_NUM_POINTS; i++) {
        int nexti_cyclic = (i+1) % __ROCKET_NUM_POINTS; // cyclic so we can link the first and last points.
        ROCKET_EXPLODED_POINTS[i][0] = ROCKET_POINTS[i];
        ROCKET_EXPLODED_POINTS[i][1] = ROCKET_POINTS[nexti_cyclic];
    
        // Constructs orthogonal vector to currently selected exploded side
        struct vector sidev = vec_difference(ROCKET_EXPLODED_POINTS[i][0], ROCKET_EXPLODED_POINTS[i][1]);
        struct vector side_orthov = vec_orthogonal(sidev);
        // Scales that orthogonal vector to the speed at which the sides are flying out

        struct vector vn = vec_normalize(side_orthov);
        side_orthov.x = ROCKET_SIDES_DRIFT_SPEED * vn.x;
        side_orthov.y = ROCKET_SIDES_DRIFT_SPEED * vn.y;
        ROCKET_SIDES_DRIFT_VECS[i] = side_orthov; // assigns this sides' stored drift vector to what we calculated
    }
    ROCKET_EXPLODED_SIDES_NUM_POINTS = 2; // each exploded side has 2 points in it to draw.
    ROCKET_NUM_POINTS = 0; // erases rocket by zeroing the number of its points in the eyes of the game draw loop
}

void rocket_explode_update() {
    // Exploded sides position updating loop should not activate unless rocket_explode_init() previously called.
    for(int i = 0; i < __ROCKET_NUM_POINTS*(ROCKET_EXPLODED_SIDES_NUM_POINTS/2); i++) {
        // #pragma unroll GCC 2
        for(int j = 0; j < 2; j++) {
            // Makes rocket exploded side points drift as intended.
            ROCKET_EXPLODED_POINTS[i][j].x += ROCKET_SIDES_DRIFT_VECS[i].x;
            ROCKET_EXPLODED_POINTS[i][j].y += ROCKET_SIDES_DRIFT_VECS[i].y;
        }
    }
}
