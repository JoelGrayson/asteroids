#include "rocket.h"
#include "mechanics.h"
#include "constants.h"
#include "printf.h"

int ROCKET_NUM_POINTS = __ROCKET_NUM_POINTS;
int ROCKET_EXPLODED_SIDES_NUM_POINTS = 0;

struct vector ROCKET_SIDES_DRIFT_VECS[__ROCKET_NUM_POINTS-1];

// Rocket points, polygon.
struct point ROCKET_POINTS[__ROCKET_NUM_POINTS] = {
    { 25 - 25, 5  - 25 },
    { 12 - 25, 45 - 25 },
    { 15 - 25, 36 - 25 },
    { 35 - 25, 36 - 25 },
    { 38 - 25, 45 - 25 },
    { 25 - 25, 5  - 25 }
};

// Array of rocket sides polygons.
struct point ROCKET_EXPLODED_POINTS[__ROCKET_NUM_POINTS-1][2] = {
    {   {25 - 25, 5 - 25}, {12 - 25, 45 - 25}     },
    {   {12 - 25, 45 - 25}, {15 - 25, 36 - 25}    },
    {   {15 - 25, 36 - 25}, {35 - 25, 36 - 25}    },
    {   {35 - 25, 36 - 25}, {38 - 25, 45 - 25}    },
    {   {38 - 25, 45 - 25}, {25 - 25, 5 - 25}     }
};

struct mechanics rocket_mechanics = {
    .x = MONITOR_WIDTH / 2,
    .y = MONITOR_HEIGHT / 2,
    .vx = 0,
    .vy = 0,
    .ax = 0,
    .ay = 0,
    .rotation = 0
};

struct rocket {
    int num_points = ROCKET_NUM_POINTS,
    struct point rotated_points[]
    // rocket points = constant (just facing north)
    // rotated_points = re-caluclated every frame by calling rotate_points(rocket_points)
    // mechanics
};

struct rocket rocket_object;

void rocket_init() {
    rocket_object.
}

void rocket_explode_init() {
    // Copies present positions of rocket sides to exploded sides arrays in order to isolate the sides
    // each in their own polygon array able to fly off in different directions.
    for(int i = 0; i < __ROCKET_NUM_POINTS-1; i++) {
        int nexti_cyclic = (i+1) % __ROCKET_NUM_POINTS; // cyclic so we can link the first and last points.
        //printf("i is %d, nexti_cyclic is %d\n", i, nexti_cyclic);
        ROCKET_EXPLODED_POINTS[i][0] = ROCKET_POINTS[i];
        ROCKET_EXPLODED_POINTS[i][1] = ROCKET_POINTS[nexti_cyclic];
        
        //printf("ROCKET_POINTS[%d] is (%f, %f)\n", i, ROCKET_POINTS[i].x, ROCKET_POINTS[i].y); 
        //printf("ROCKET_POINTS[%d] is (%f, %f)\n", nexti_cyclic, ROCKET_POINTS[nexti_cyclic].x, ROCKET_POINTS[nexti_cyclic].y);
        
        // Constructs orthogonal vector to currently selected exploded side
        struct vector sidev = vec_difference(ROCKET_EXPLODED_POINTS[i][0], ROCKET_EXPLODED_POINTS[i][1]);

        //printf("sidev is (%f, %f)\n", sidev.x, sidev.y);
        struct vector side_orthov = vec_orthogonal(sidev);
        //printf("side_orthov is (%f, %f)\n", side_orthov.x, side_orthov.y);
        // Scales that orthogonal vector to the speed at which the sides are flying out

        struct vector vn = vec_normalize(side_orthov);
        //printf("vn is (%f, %f)\n", vn.x, vn.y);
        printf("DRIFT SPEED IS %f\n", ROCKET_SIDES_DRIFT_SPEED/100.0);
        side_orthov.x = (double)ROCKET_SIDES_DRIFT_SPEED/100.0 * vn.x;
        side_orthov.y = (double)ROCKET_SIDES_DRIFT_SPEED/100.0 * vn.y;
        ROCKET_SIDES_DRIFT_VECS[i] = side_orthov; // assigns this sides' stored drift vector to what we calculated
    }
    ROCKET_EXPLODED_SIDES_NUM_POINTS = 2; // each exploded side has 2 points in it to draw.
    ROCKET_NUM_POINTS = 0; // erases rocket by zeroing the number of its points in the eyes of the game draw loop
}

void rocket_explode_update() {
    // Exploded sides position updating loop should not activate unless rocket_explode_init() previously called.
    for(int i = 0; i < (__ROCKET_NUM_POINTS-1)*(ROCKET_EXPLODED_SIDES_NUM_POINTS/2); i++) {
        // #pragma unroll GCC 2
        for(int j = 0; j < 2; j++) {
            //printf("ROCKET SIDES DRIFT VECS IS (%f, %f)\n", ROCKET_SIDES_DRIFT_VECS[i].x, ROCKET_SIDES_DRIFT_VECS[i].y);
            // Makes rocket exploded side points drift as intended.
            ROCKET_EXPLODED_POINTS[i][j].x += ROCKET_SIDES_DRIFT_VECS[i].x;
            ROCKET_EXPLODED_POINTS[i][j].y += ROCKET_SIDES_DRIFT_VECS[i].y;
        }
    }
}

void rocket_update_mechanics() {
    // TODO: bounds checking
    update_mechanics(&rocket_mechanics);
    if (rocket_mechanics.x < 0) {
        rocket_mechanics.x = MONITOR_WIDTH;
    }
    if (rocket_mechanics.x > MONITOR_WIDTH) {
        rocket_mechanics.x = 0;
    }
    
    if (rocket_mechanics.y < 0) {
        rocket_mechanics.y = MONITOR_HEIGHT;
    }
    if (rocket_mechanics.y > MONITOR_HEIGHT) {
        rocket_mechanics.y = 0;
    }

}


