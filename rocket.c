#include <stdbool.h>
#include "rocket.h"
#include "graphics/geometry.h"
#include "graphics/rotate_vector.h"
#include "maths.h"
#include "timer.h"
#include "rand.h"

#define ROCKET_DECELERATION 0.15

// Points of a rocket facing north
static struct point ROCKET_POINTS[ROCKET_NUM_POINTS] = {
    { 25 - 25, 5  - 25 },
    { 12 - 25, 45 - 25 },
    { 15 - 25, 36 - 25 },
    { 35 - 25, 36 - 25 },
    { 38 - 25, 45 - 25 },
    { 25 - 25, 5  - 25 }
};

// Starts off as the same as ROCKET_POINTS
static struct point rotated_rocket_points[ROCKET_NUM_POINTS] = {
    { 25 - 25, 5  - 25 },
    { 12 - 25, 45 - 25 },
    { 15 - 25, 36 - 25 },
    { 35 - 25, 36 - 25 },
    { 38 - 25, 45 - 25 },
    { 25 - 25, 5  - 25 }
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


static bool rocket_is_exploding = false;
static bool rocket_is_thrusting = false;

// Increases when rocket_is_exploding == true. Determines how far off the rocket segments are from each other when rendering
static int num_frames_after_rocket_exploded = 0;


void rocket_rotate_left() {
    rocket_rotate_radians(-0.1);
    (void) num_frames_after_rocket_exploded;
}

void rocket_rotate_right() {
    rocket_rotate_radians(+0.1);
}

void rocket_fire() {
    // TODO: create a bullet
}

void rocket_thrust() {
    rocket_is_thrusting = true;
}

void rocket_unthrust() {
    rocket_is_thrusting = false;
}

void rocket_hyperspace() {
    // Teleports to a "random" location within the screen bounds after seeding random with the current tick count.
    unsigned int ticks = (unsigned int)timer_get_ticks();
    srand(ticks);
    rocket_mechanics.x = (double)(22 + (rand() % (unsigned int)(MONITOR_WIDTH-44))); // offset of 22 from edges to ensure rocket is fully rendered within screen.
    rocket_mechanics.y = (double)(22 + (rand() % (unsigned int)(MONITOR_HEIGHT-44)));
}

struct mechanics get_rocket_mechanics() {
    return rocket_mechanics;
}


void rocket_rotate_radians(double theta) {
    rocket_mechanics.rotation += theta;

    rotate_template_points(
        rotated_rocket_points,
        ROCKET_POINTS,
        ROCKET_NUM_POINTS,
        rocket_mechanics.rotation
    );
}

// Draws the rocket
void render_rocket() {
    if (rocket_is_exploding) {

    } else {
        // Normal rocket
        draw_points(rotated_rocket_points, ROCKET_NUM_POINTS, rocket_mechanics.x, rocket_mechanics.y, GL_WHITE);
    }
}


void explode_rocket() {
    rocket_is_exploding = true;
}

struct point *get_points_of_rocket() {
    return rotated_rocket_points;
}

int get_num_rocket_points() {
    return ROCKET_NUM_POINTS;
}













// struct vector ROCKET_SIDES_DRIFT_VECS[__ROCKET_NUM_POINTS-1];

// // Rocket points, polygon.


// // Array of rocket sides polygons.
// struct point ROCKET_EXPLODED_POINTS[__ROCKET_NUM_POINTS-1][2] = {
//     {   {25 - 25, 5 - 25}, {12 - 25, 45 - 25}     },
//     {   {12 - 25, 45 - 25}, {15 - 25, 36 - 25}    },
//     {   {15 - 25, 36 - 25}, {35 - 25, 36 - 25}    },
//     {   {35 - 25, 36 - 25}, {38 - 25, 45 - 25}    },
//     {   {38 - 25, 45 - 25}, {25 - 25, 5 - 25}     }
// };


// struct rocket {
//     int num_points = ROCKET_NUM_POINTS,
//     struct point rotated_points[]
//     // rocket points = constant (just facing north)
//     // rotated_points = re-caluclated every frame by calling rotate_points(rocket_points)
//     // mechanics
// };

// struct rocket rocket_object;

// void rocket_init() {
//     rocket_object.
// }

// void rocket_explode_init() {
//     // Copies present positions of rocket sides to exploded sides arrays in order to isolate the sides
//     // each in their own polygon array able to fly off in different directions.
//     for(int i = 0; i < __ROCKET_NUM_POINTS-1; i++) {
//         int nexti_cyclic = (i+1) % __ROCKET_NUM_POINTS; // cyclic so we can link the first and last points.
//         //printf("i is %d, nexti_cyclic is %d\n", i, nexti_cyclic);
//         ROCKET_EXPLODED_POINTS[i][0] = ROCKET_POINTS[i];
//         ROCKET_EXPLODED_POINTS[i][1] = ROCKET_POINTS[nexti_cyclic];
        
//         //printf("ROCKET_POINTS[%d] is (%f, %f)\n", i, ROCKET_POINTS[i].x, ROCKET_POINTS[i].y); 
//         //printf("ROCKET_POINTS[%d] is (%f, %f)\n", nexti_cyclic, ROCKET_POINTS[nexti_cyclic].x, ROCKET_POINTS[nexti_cyclic].y);
        
//         // Constructs orthogonal vector to currently selected exploded side
//         struct vector sidev = vec_difference(ROCKET_EXPLODED_POINTS[i][0], ROCKET_EXPLODED_POINTS[i][1]);

//         //printf("sidev is (%f, %f)\n", sidev.x, sidev.y);
//         struct vector side_orthov = vec_orthogonal(sidev);
//         //printf("side_orthov is (%f, %f)\n", side_orthov.x, side_orthov.y);
//         // Scales that orthogonal vector to the speed at which the sides are flying out

//         struct vector vn = vec_normalize(side_orthov);
//         //printf("vn is (%f, %f)\n", vn.x, vn.y);
//         printf("DRIFT SPEED IS %f\n", ROCKET_SIDES_DRIFT_SPEED/100.0);
//         side_orthov.x = (double)ROCKET_SIDES_DRIFT_SPEED/100.0 * vn.x;
//         side_orthov.y = (double)ROCKET_SIDES_DRIFT_SPEED/100.0 * vn.y;
//         ROCKET_SIDES_DRIFT_VECS[i] = side_orthov; // assigns this sides' stored drift vector to what we calculated
//     }
//     ROCKET_EXPLODED_SIDES_NUM_POINTS = 2; // each exploded side has 2 points in it to draw.
//     ROCKET_NUM_POINTS = 0; // erases rocket by zeroing the number of its points in the eyes of the game draw loop
// }

// void rocket_explode_update() {
//     // Exploded sides position updating loop should not activate unless rocket_explode_init() previously called.
//     for(int i = 0; i < (__ROCKET_NUM_POINTS-1)*(ROCKET_EXPLODED_SIDES_NUM_POINTS/2); i++) {
//         // #pragma unroll GCC 2
//         for(int j = 0; j < 2; j++) {
//             //printf("ROCKET SIDES DRIFT VECS IS (%f, %f)\n", ROCKET_SIDES_DRIFT_VECS[i].x, ROCKET_SIDES_DRIFT_VECS[i].y);
//             // Makes rocket exploded side points drift as intended.
//             ROCKET_EXPLODED_POINTS[i][j].x += ROCKET_SIDES_DRIFT_VECS[i].x;
//             ROCKET_EXPLODED_POINTS[i][j].y += ROCKET_SIDES_DRIFT_VECS[i].y;
//         }
//     }
// }

void rocket_update_mechanics() {
    struct mechanics *mech = &rocket_mechanics;
    if(rocket_is_thrusting) {
        // Rocket thrusts with magnitude 1 acceleration in the direction of its cone
        struct vector dir = {0, -1};
        rotate_vector(&dir, mech->rotation);
        mech->ax = dir.x;
        mech->ay = dir.y;
    } else {
        // If the rocket isn't thrusting, set acceleration to zero and manually decelerate using the if blocks below, since we don't want deceleration to
        // accidentally push the rocket into negative acceleration of its previous heading.
        mech->ax = 0;
        mech->ay = 0;
        if(abs(mech->vx) >= ROCKET_DECELERATION) {
            mech->vx = (mech->vx < 0) ? mech->vx + ROCKET_DECELERATION : mech->vx - ROCKET_DECELERATION;
        } else {
            mech->vx = 0;
        }
        if(abs(mech->vy) >= ROCKET_DECELERATION) {
            mech->vy = (mech->vy < 0) ? mech->vy + ROCKET_DECELERATION : mech->vy - ROCKET_DECELERATION;
        } else {
            mech->vy = 0;
        }
    }
    update_mechanics(&rocket_mechanics, true);
}


