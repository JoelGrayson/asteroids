#include "asteroid.h"
#include "mechanics.h"
#include "constants.h"
#include "graphics/point.h"
#include "graphics/geometry.h"
#include "rand.h"

// Gets the asteroid polygon points
struct point *get_points_of_asteroid(struct asteroid ast) {
    if (ast.type == A) {
        if (ast.size == BIG)    return ASTEROID_A_BIG_POINTS;
        if (ast.size == MEDIUM) return ASTEROID_A_MEDIUM_POINTS;
        if (ast.size == SMALL)  return ASTEROID_A_SMALL_POINTS;
    }
    if (ast.type == B) {
        if (ast.size == BIG)    return ASTEROID_B_BIG_POINTS;
        if (ast.size == MEDIUM) return ASTEROID_B_MEDIUM_POINTS;
        if (ast.size == SMALL)  return ASTEROID_B_SMALL_POINTS;
    }
    if (ast.type == C) {
        if (ast.size == BIG)    return ASTEROID_C_BIG_POINTS;
        if (ast.size == MEDIUM) return ASTEROID_C_MEDIUM_POINTS;
        if (ast.size == SMALL)  return ASTEROID_C_SMALL_POINTS;
    }
    printf("Error: get_points_of_asteroid: invalid asteroid\n");
    return ASTEROID_A_BIG_POINTS; //should not happen
}

// Mechanics function for asteroids during game loop
// void asteroids_update_mechanics(struct asteroids* asteroids, int num_asteroids) {
//     for(int i = 0; i < num_asteroids; i++) {
//         // Asteroid continues on its present course
//         update_mechanics(&asteroids[i].mechanics); // Any object in motion at a certain velocity will remain in motion at that velocity (Newton's 1st Law).
//         // Respawns asteroid if out-of-bounds
//         struct point pos = asteroid_get_pos(asteroids[i]);
//         if(pos.x < 0 || pos.x > MONITOR_WIDTH || pos.y < 0 || pos.y > MONITOR_HEIGHT) {
//             asteroid_respawn(&asteroid[i]);
//         }
//     }
// }

// Returns position of asteroid in a more convenient way
struct point asteroid_get_pos(struct asteroid ast) {
    struct point ret = {ast.mechanics.x, ast.mechanics.y};
    return ret;
}

// Sets position of asteroid in a more convenient way
struct point asteroid_set_pos(struct asteroid* ast, struct point new_pos) {
    ast->mechanics.x = new_pos.x;
    ast->mechanics.y = new_pos.y;
}

// Respawns asteroid at a random point along the edge of the screen
void asteroid_respawn(struct asteroid* ast, unsigned int MAX_ASTEROID_SPEED) {
    int edge_axis = rand() % 3;    // Selects the edge we respawn the asteroid on by random lot
    struct point respawn_pos;      // Position of asteroid respawn (on edge of screen, top, bottom, left, right)
    struct vector respawn_heading; // Heading/velocity of asteroid upon respawn
    switch(edge_axis) { // Sets initial edge positions of asteroids by clockwise edge asteroid starts out hugging.
        case 0: // Top edge
            respawn_pos.y = 1;
            respawn_pos.x = 1+(rand() % (MONITOR_WIDTH - 2));

            respawn_heading.x = rand() % MAX_ASTEROID_SPEED;
            if((rand() % 1)) respawn_heading.x *= -1;

            respawn_heading.y = rand() % MAX_ASTEROID_SPEED;
            break;
        case 1: // Right edge
            respawn_pos.x = MONITOR_WIDTH-1;
            respawn_pos.y = 1+(rand() % (MONITOR_HEIGHT - 2));

            respawn_heading.x = -1*(int)(rand() % MAX_ASTEROID_SPEED);
            
            respawn_heading.y = rand() % MAX_ASTEROID_SPEED;
            if((rand() % 1)) respawn_heading.y *= -1;
            break;
        case 2: // Bottom edge
            respawn_pos.y = MONITOR_HEIGHT-1;
            respawn_pos.x = 1+(rand() % (MONITOR_WIDTH - 2));

            respawn_heading.x = rand() % MAX_ASTEROID_SPEED;
            if((rand() % 1)) respawn_heading.x *= -1;
            
            respawn_heading.y = -1*(int)(rand() % MAX_ASTEROID_SPEED);
            break;
        case 3: // Left edge
            respawn_pos.x = 1;
            respawn_pos.y = 1+(rand() % (MONITOR_HEIGHT - 2));

            respawn_heading.x = rand() % MAX_ASTEROID_SPEED;
            
            respawn_heading.y = rand() % MAX_ASTEROID_SPEED;
            if((rand() % 1)) respawn_heading.y *= -1;
            break;
    }
    // Updates mechanics position and velocities of asteroid to reflect its respawned position and heading.
    ast->mechanics.x = respawn_pos.x;
    ast->mechanics.y = respawn_pos.y;

    ast->mechanics.vx = respawn_heading.x;
    ast->mechanics.vy = respawn_heading.y;
}

struct point ASTEROID_A_SMALL_POINTS[ASTEROID_NUM_POINTS] = {
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
    { 10, 5 },
};

struct point ASTEROID_B_SMALL_POINTS[ASTEROID_NUM_POINTS] = {
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
    { 4, 14 },
    { 4, 10 },
    { 10, 5 },
};

struct point ASTEROID_C_SMALL_POINTS[ASTEROID_NUM_POINTS] = {
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
    { 10, 5 },
};


struct point ASTEROID_A_MEDIUM_POINTS[ASTEROID_NUM_POINTS] = {
    { 10 * 2, 5 * 2 },
    { 19 * 2, 5 * 2 },
    { 27 * 2, 9 * 2 },
    { 27 * 2, 12 * 2 },
    { 19 * 2, 14 * 2 },
    { 27 * 2, 19 * 2 },
    { 21 * 2, 24 * 2 },
    { 18 * 2, 21 * 2 },
    { 10 * 2, 24 * 2 },
    { 4 * 2, 17 * 2 },
    { 4 * 2, 9 * 2 },
    { 13 * 2, 9 * 2 },
    { 10 * 2, 5 * 2 },
};

struct point ASTEROID_B_MEDIUM_POINTS[ASTEROID_NUM_POINTS] = {
    { 10 * 2, 5 * 2 },
    { 15 * 2, 10 * 2 },
    { 21 * 2, 5 * 2 },
    { 26 * 2, 10 * 2 },
    { 23 * 2, 15 * 2 },
    { 26 * 2, 20 * 2 },
    { 18 * 2, 25 * 2 },
    { 10 * 2, 25 * 2 },
    { 4 * 2, 20 * 2 },
    { 4 * 2, 17 * 2 },
    { 4 * 2, 14 * 2 },
    { 4 * 2, 10 * 2 },
    { 10 * 2, 5 * 2 },
};

struct point ASTEROID_C_MEDIUM_POINTS[ASTEROID_NUM_POINTS] = {
    { 10 * 2, 5 * 2 },
    { 4 * 2, 10 * 2 },
    { 7 * 2, 15 * 2 },
    { 4 * 2, 19 * 2 },
    { 9 * 2, 24 * 2 },
    { 13 * 2, 21 * 2 },
    { 21 * 2, 24 * 2 },
    { 26 * 2, 17 * 2 },
    { 21 * 2, 13 * 2 },
    { 26 * 2, 9 * 2 },
    { 21 * 2, 5 * 2 },
    { 15 * 2, 7 * 2 },
    { 10 * 2, 5 * 2 },
};


struct point ASTEROID_A_BIG_POINTS[ASTEROID_NUM_POINTS] = {
    { 10 * 4, 5 * 4 },
    { 19 * 4, 5 * 4 },
    { 27 * 4, 9 * 4 },
    { 27 * 4, 12 * 4 },
    { 19 * 4, 14 * 4 },
    { 27 * 4, 19 * 4 },
    { 21 * 4, 24 * 4 },
    { 18 * 4, 21 * 4 },
    { 10 * 4, 24 * 4 },
    { 4 * 4, 17 * 4 },
    { 4 * 4, 9 * 4 },
    { 13 * 4, 9 * 4 },
    { 10 * 4, 5 * 4 },
};

struct point ASTEROID_B_BIG_POINTS[ASTEROID_NUM_POINTS] = {
    { 10 * 4, 5 * 4 },
    { 15 * 4, 10 * 4 },
    { 21 * 4, 5 * 4 },
    { 26 * 4, 10 * 4 },
    { 23 * 4, 15 * 4 },
    { 26 * 4, 20 * 4 },
    { 18 * 4, 25 * 4 },
    { 10 * 4, 25 * 4 },
    { 4 * 4, 20 * 4 },
    { 4 * 4, 17 * 4 },
    { 4 * 4, 14 * 4 },
    { 4 * 4, 10 * 4 },
    { 10 * 4, 5 * 4 },
};

struct point ASTEROID_C_BIG_POINTS[ASTEROID_NUM_POINTS] = {
    { 10 * 4, 5 * 4 },
    { 4 * 4, 10 * 4 },
    { 7 * 4, 15 * 4 },
    { 4 * 4, 19 * 4 },
    { 9 * 4, 24 * 4 },
    { 13 * 4, 21 * 4 },
    { 21 * 4, 24 * 4 },
    { 26 * 4, 17 * 4 },
    { 21 * 4, 13 * 4 },
    { 26 * 4, 9 * 4 },
    { 21 * 4, 5 * 4 },
    { 15 * 4, 7 * 4 },
    { 10 * 4, 5 * 4 },
};

