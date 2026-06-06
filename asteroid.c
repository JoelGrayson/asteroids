#include "asteroid.h"

// Maximum asteroid speed global variable -- should increase as game progresses in time (and thereby difficulty).
unsigned int MAX_ASTEROID_SPEED = 15;

// List of asteroids tracked in program
static struct asteroid_list_t list[MAX_NUM_ASTEROIDS];
// Number of asteroids to be tracked
static int list_size = MAX_NUM_ASTEROIDS;

// Asteroid explosion audio playback tracking to make sure we don't have silly playback issues.
unsigned long last_explosion_sound_tick;

extern unsigned long last_fire_sound_tick;
extern unsigned long last_thrust_sound_tick;

// Minimum asteroid speed define
#define MIN_ASTEROID_SPEED 2

static long frame_when_asteroid_last_spawned = 0;
static int num_frames_between_spawn = 2 * FPS; //every 3 seconds spawn a new asteroid. This time decreases as the score increases
static int noisy_num_frames_between_spawn = 2 * FPS; //±80% of num_frames_between_spawn. Updated every time a new asteroid is spawned

static void spawn_asteroids_if_necessary();
static void asteroid_spawn();
static void asteroid_custom_spawn(struct point loc, struct vector heading, enum asteroid_type type, enum asteroid_size size); // Allows us to spawn an asteroid with custom parameters.
static void asteroid_despawn(struct asteroid_list_t* ast);
static struct asteroid_list_t *get_next_spawnable_asteroid();

void setup_asteroids() {
    // Spawn 9 asteroids at the start of the game
    for(int i = 0; i < 9; i++) {
        asteroid_spawn();
        frame_when_asteroid_last_spawned = 0;
    }
}

// Gets the asteroid polygon points
static struct point *get_points_of_asteroid(struct asteroid ast) {
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

// Returns position of asteroid in a more convenient way
static struct point asteroid_get_pos(struct asteroid ast) {
    return mechanics_to_point(ast.mechanics);
}

// Sets position of asteroid in a more convenient way
static void asteroid_set_pos(struct asteroid* ast, struct point new_pos) {
    ast->mechanics.x = new_pos.x;
    ast->mechanics.y = new_pos.y;
}

// Mechanics function for asteroids during game loop
static void asteroids_update_mechanics() {
    for(int i = 0; i < list_size; i++) {
        if(list[i].allocated) { // Only update mechanics
            // If exploding, increment the exploding frame so animation precedes
            if (list[i].ast.is_exploding) {
                list[i].ast.exploding_frame++;
            }
            // Done exploding animation
            if (list[i].ast.exploding_frame > NUM_FRAMES_OF_EXPLOSION) {
                list[i].ast.exploding_frame = 0;
                list[i].ast.is_exploding = false;
                asteroid_despawn(&list[i]);
            }
            
            // Asteroid continues on its present course
            update_mechanics(&list[i].ast.mechanics, false); // Any object in motion at a certain velocity will remain in motion at that velocity (Newton's 1st Law).
            // Respawns asteroid if out-of-bounds
            struct point pos = asteroid_get_pos(list[i].ast);
            if(pos.x < -60 || pos.x > MONITOR_WIDTH+60 || pos.y < -60 || pos.y > MONITOR_HEIGHT+60) { // Asteroids ~ 60 radius max
                asteroid_despawn(&list[i]);
            }
        }
    }
}

// Respawns asteroid at a random point along the edge of the screen
static void asteroid_spawn() {
    // Locates next spawnable asteroid from our list -- just returns function if there is no space for any more asteroids.
    struct asteroid_list_t *ast = get_next_spawnable_asteroid(list, list_size);
    if(ast == NULL) return;

    int edge_axis = rand() % 3;    // Selects the edge we respawn the asteroid on by random lot
    struct point respawn_pos;      // Position of asteroid respawn (on edge of screen, top, bottom, left, right)
    struct vector respawn_heading = {0, 0}; // Heading/velocity of asteroid upon respawn
    
    switch(edge_axis) { // Sets initial edge positions of asteroids by clockwise edge asteroid starts out hugging.
        case 0: // Top edge
            respawn_pos.y = 1;
            respawn_pos.x = 1+(rand() % (unsigned int)(MONITOR_WIDTH - 2));

            while(respawn_heading.x < MIN_ASTEROID_SPEED) {
                respawn_heading.x = (double)(rand() % (MAX_ASTEROID_SPEED*100))/100;
            }
            if((rand() % 2)) respawn_heading.x *= -1;

            while(respawn_heading.y < MIN_ASTEROID_SPEED) {
                respawn_heading.y = (double)(rand() % (MAX_ASTEROID_SPEED*100))/100;
            }
            break;
        case 1: // Right edge
            respawn_pos.x = MONITOR_WIDTH-1;
            respawn_pos.y = 1+(rand() % (unsigned int)((MONITOR_HEIGHT - 2)));

            while(respawn_heading.x < MIN_ASTEROID_SPEED) {
                respawn_heading.x = (double)(rand() % (MAX_ASTEROID_SPEED*100))/100;
            }
            respawn_heading.x *= -1;
            
            while(respawn_heading.y < MIN_ASTEROID_SPEED) {
                respawn_heading.y = (double)(rand() % (MAX_ASTEROID_SPEED*100))/100;
            }
            if((rand() % 2)) respawn_heading.y *= -1;
            break;
        case 2: // Bottom edge
            respawn_pos.y = MONITOR_HEIGHT-1;
            respawn_pos.x = 1+(rand() % (unsigned int)(MONITOR_WIDTH - 2));

            while(respawn_heading.x < MIN_ASTEROID_SPEED) {
                respawn_heading.x = (double)(rand() % (MAX_ASTEROID_SPEED*100))/100;
            }
            if((rand() % 2)) respawn_heading.x *= -1;
            
            while(respawn_heading.y < MIN_ASTEROID_SPEED) {
                respawn_heading.y = (double)(rand() % (MAX_ASTEROID_SPEED*100))/100;
            }
            respawn_heading.y *= -1;
            break;
        case 3: // Left edge
            respawn_pos.x = 1;
            respawn_pos.y = 1+(rand() % (unsigned int)(MONITOR_HEIGHT - 2));

            while(respawn_heading.x < MIN_ASTEROID_SPEED) {
                respawn_heading.x = (double)(rand() % (MAX_ASTEROID_SPEED*100))/100;
            }
            
            while(respawn_heading.y < MIN_ASTEROID_SPEED) {
                respawn_heading.y = (double)(rand() % (MAX_ASTEROID_SPEED*100))/100;
            }
            if((rand() % 2)) respawn_heading.y *= -1;
            break;
    }
    // Updates mechanics position and velocities of asteroid to reflect its respawned position and heading.
    ast->ast.mechanics.x = respawn_pos.x;
    ast->ast.mechanics.y = respawn_pos.y;

    ast->ast.mechanics.vx = respawn_heading.x;
    ast->ast.mechanics.vy = respawn_heading.y;

    // Randomizes spawned asteroid type and size
    enum asteroid_type type = A + (rand() % 3);
    enum asteroid_size size = BIG + (rand() % 3);

    ast->ast.type = type;
    ast->ast.size = size;

    ast->allocated = true; // We have just spawned this asteroid, so it is now allocated in the asteroid list.
}

static void asteroid_custom_spawn(struct point loc, struct vector heading, enum asteroid_type type, enum asteroid_size size) {
    // Locates next spawnable asteroid from our list -- just returns function if there is no space for any more asteroids.
    struct asteroid_list_t *ast = get_next_spawnable_asteroid(list, list_size);
    if(ast == NULL) return;

    ast->ast.mechanics.x = loc.x;
    ast->ast.mechanics.y = loc.y;

    ast->ast.mechanics.vx = heading.x;
    ast->ast.mechanics.vy = heading.y;

    ast->ast.type = type;
    ast->ast.size = size;
    ast->allocated = true; // We have just spawned this asteroid, so it is now allocated in the asteroid list.
}

// Despawns asteroid by setting it to "deallocated."
static void asteroid_despawn(struct asteroid_list_t* ast) {
    ast->allocated = false;
}

// Finds the pointer of the next asteroid which can be allocated out of the list of asteroids.
static struct asteroid_list_t *get_next_spawnable_asteroid() {
    // Return the location of the next unallocated asteroid in the list if it can be found, null otherwise.
    for(int i = 0; i < list_size; i++) {
        if(!list[i].allocated) return &list[i];
    }
    return NULL;
}

static void render_asteroids() {
    // Draws all allocated asteroids
    for(int i = 0; i < list_size; i++) {
        if(list[i].allocated) {
            //printf("something is allocated\n");
            struct asteroid a = list[i].ast;
            if (a.is_exploding) {
                render_explosion(mechanics_to_point(a.mechanics), a.exploding_frame);
            } else {
                struct point *points = get_points_of_asteroid(a);
                draw_points(points, ASTEROID_NUM_POINTS, a.mechanics.x, a.mechanics.y, GL_WHITE);
            }
        }
    }
}

void loop_asteroids(long frame) {
    render_asteroids();
    asteroids_update_mechanics();
    spawn_asteroids_if_necessary(frame);
}

static void spawn_asteroids_if_necessary(long frame) {
    // Spawns new asteroid at regularly spaced intervals
    int frames_since_spawn = frame - frame_when_asteroid_last_spawned;
    if (frames_since_spawn > noisy_num_frames_between_spawn) {
        asteroid_spawn();

        
        frame_when_asteroid_last_spawned = frame;
        
        int one_to_ten = (rand() % 10); //random number from 1 to 10
        double noise_ratio = 0.5 + (double)one_to_ten / 10.0; //0.5 to 1.5

        noisy_num_frames_between_spawn = num_frames_between_spawn * noise_ratio;
    }
}

struct asteroid_list_t *get_asteroid_collision(struct mechanics obj, struct point *points_obj, int num_points_obj) {
    // Checks all allocated asteroids for collisions with object:
    for(int i = 0; i < list_size; i++) {
        if(list[i].allocated) {
            struct asteroid a = list[i].ast;
            struct point *points = get_points_of_asteroid(a);
            if(are_colliding(a.mechanics, obj, points, points_obj, ASTEROID_NUM_POINTS, num_points_obj)) return &list[i];
        }
    }
    return NULL; // No asteroids found colliding with our object.
}

void asteroid_increase_score_by(enum asteroid_size asteroid_size) {
    switch (asteroid_size) {
        case BIG:
            increase_score_by(20);
            break;
        case MEDIUM:
            increase_score_by(50);
            break;
        case SMALL:
            increase_score_by(100);
            break;
    }
}

void asteroid_explode(struct asteroid_list_t *ast) {
    // Explosion sound-playing code:
    unsigned long present_tick = timer_get_ticks();
    if(present_tick-last_fire_sound_tick > FIRE_SOUND_TICK_DURATION && present_tick-last_thrust_sound_tick > THRUST_SOUND_TICK_DURATION && present_tick-last_explosion_sound_tick > EXPLOSION_SOUND_TICK_DURATION) {
        switch(ast->ast.size) {
            case BIG:
                increase_score_by(20);
                play_bangLarge();
                break;
            case MEDIUM:
                play_bangMedium();
                increase_score_by(50);
                break;
            case SMALL:
                play_bangSmall();
                increase_score_by(100);
                break;
        }
        last_explosion_sound_tick = timer_get_ticks();
    }
    if(ast->ast.size == SMALL) { // Just blow up the asteroid into constituent particles.
        ast->allocated = false;
        ast->ast.is_exploding = true;
        ast->ast.exploding_frame = 0;
    } else {
        ast->ast.size += 1; // Decrement asteroid size from BIG to MEDIUM, or from MEDIUM to SMALL.
        struct point ast_loc = {ast->ast.mechanics.x, ast->ast.mechanics.y};

        /*
        // Adds a bit of random variation into our explosions, to keep them interesting!
        double headingvx_dev = (double)((rand() % 5) - 2);
        double headingvy_dev = (double)((rand() % 5) - 2);

        double new_headingvx_dev = (double)((rand() % 5) - 3);
        double new_headingvy_dev = (double)((rand() % 5) - 3);
        */

        struct vector new_heading = {(-1*ast->ast.mechanics.vx)/*+new_headingvx_dev*/, (-1*ast->ast.mechanics.vy)/*+new_headingvy_dev*/};
        /*ast->ast.mechanics.vx += headingvx_dev;
        ast->ast.mechanics.vy += headingvy_dev;*/

        // Randomizes spawned asteroid type.
        enum asteroid_type type = A + (rand() % 3);
        asteroid_custom_spawn(ast_loc, new_heading, type, ast->ast.size);
    }
}

struct point ASTEROID_A_SMALL_POINTS[ASTEROID_NUM_POINTS] = {
    { 10 - 16, 5 - 14 },
    { 19 - 16, 5 - 14 },
    { 27 - 16, 9 - 14 },
    { 27 - 16, 12 - 14 },
    { 19 - 16, 14 - 14 },
    { 27 - 16, 19 - 14 },
    { 21 - 16, 24 - 14 },
    { 18 - 16, 21 - 14 },
    { 10 - 16, 24 - 14 },
    { 4 - 16, 17 - 14 },
    { 4 - 16, 9 - 14 },
    { 13 - 16, 9 - 14 },
    { 10 - 16, 5 - 14 },
};

struct point ASTEROID_B_SMALL_POINTS[ASTEROID_NUM_POINTS] = {
    { 10 - 14,  5 - 14 },
    { 15 - 14, 10 - 14 },
    { 21 - 14,  5 - 14 },
    { 26 - 14, 10 - 14 },
    { 23 - 14, 15 - 14 },
    { 26 - 14, 20 - 14 },
    { 18 - 14, 25 - 14 },
    { 10 - 14, 25 - 14 },
    { 4 - 14, 20 - 14 },
    { 4 - 14, 17 - 14 },
    { 4 - 14, 14 - 14 },
    { 4 - 14, 10 - 14 },
    { 10 - 14,  5 - 14 },
};

struct point ASTEROID_C_SMALL_POINTS[ASTEROID_NUM_POINTS] = {
    { 10 - 15,  5 - 14 },
    {  4 - 15, 10 - 14 },
    {  7 - 15, 15 - 14 },
    {  4 - 15, 19 - 14 },
    {  9 - 15, 24 - 14 },
    { 13 - 15, 21 - 14 },
    { 21 - 15, 24 - 14 },
    { 26 - 15, 17 - 14 },
    { 21 - 15, 13 - 14 },
    { 26 - 15,  9 - 14 },
    { 21 - 15,  5 - 14 },
    { 15 - 15,  7 - 14 },
    { 10 - 15,  5 - 14 },
};


struct point ASTEROID_A_MEDIUM_POINTS[ASTEROID_NUM_POINTS] = {
    { (10 - 16) * 2, (5 - 14) * 2 },
    { (19 - 16) * 2, (5 - 14) * 2 },
    { (27 - 16) * 2, (9 - 14) * 2 },
    { (27 - 16) * 2, (12 - 14) * 2 },
    { (19 - 16) * 2, (14 - 14) * 2 },
    { (27 - 16) * 2, (19 - 14) * 2 },
    { (21 - 16) * 2, (24 - 14) * 2 },
    { (18 - 16) * 2, (21 - 14) * 2 },
    { (10 - 16) * 2, (24 - 14) * 2 },
    { (4 - 16) * 2, (17 - 14) * 2 },
    { (4 - 16) * 2, (9 - 14) * 2 },
    { (13 - 16) * 2, (9 - 14) * 2 },
    { (10 - 16) * 2, (5 - 14) * 2 },
};

struct point ASTEROID_B_MEDIUM_POINTS[ASTEROID_NUM_POINTS] = {
    { (10 - 14) * 2, (5 - 14) * 2 },
    { (15 - 14) * 2, (10 - 14) * 2 },
    { (21 - 14) * 2, (5 - 14) * 2 },
    { (26 - 14) * 2, (10 - 14) * 2 },
    { (23 - 14) * 2, (15 - 14) * 2 },
    { (26 - 14) * 2, (20 - 14) * 2 },
    { (18 - 14) * 2, (25 - 14) * 2 },
    { (10 - 14) * 2, (25 - 14) * 2 },
    { (4 - 14) * 2, (20 - 14) * 2 },
    { (4 - 14) * 2, (17 - 14) * 2 },
    { (4 - 14) * 2, (14 - 14) * 2 },
    { (4 - 14) * 2, (10 - 14) * 2 },
    { (10 - 14) * 2, (5 - 14) * 2 },
};

struct point ASTEROID_C_MEDIUM_POINTS[ASTEROID_NUM_POINTS] = {
    { (10 - 15) * 2, (5 - 14) * 2 },
    { (4 - 15) * 2, (10 - 14) * 2 },
    { (7 - 15) * 2, (15 - 14) * 2 },
    { (4 - 15) * 2, (19 - 14) * 2 },
    { (9 - 15) * 2, (24 - 14) * 2 },
    { (13 - 15) * 2, (21 - 14) * 2 },
    { (21 - 15) * 2, (24 - 14) * 2 },
    { (26 - 15) * 2, (17 - 14) * 2 },
    { (21 - 15) * 2, (13 - 14) * 2 },
    { (26 - 15) * 2, (9 - 14) * 2 },
    { (21 - 15) * 2, (5 - 14) * 2 },
    { (15 - 15) * 2, (7 - 14) * 2 },
    { (10 - 15) * 2, (5 - 14) * 2 },
};


struct point ASTEROID_A_BIG_POINTS[ASTEROID_NUM_POINTS] = {
    { (10 - 16) * 4, (5 - 14) * 4 },
    { (19 - 16) * 4, (5 - 14) * 4 },
    { (27 - 16) * 4, (9 - 14) * 4 },
    { (27 - 16) * 4, (12 - 14) * 4 },
    { (19 - 16) * 4, (14 - 14) * 4 },
    { (27 - 16) * 4, (19 - 14) * 4 },
    { (21 - 16) * 4, (24 - 14) * 4 },
    { (18 - 16) * 4, (21 - 14) * 4 },
    { (10 - 16) * 4, (24 - 14) * 4 },
    { (4 - 16) * 4, (17 - 14) * 4 },
    { (4 - 16) * 4, (9 - 14) * 4 },
    { (13 - 16) * 4, (9 - 14) * 4 },
    { (10 - 16) * 4, (5 - 14) * 4 },
};

struct point ASTEROID_B_BIG_POINTS[ASTEROID_NUM_POINTS] = {
    { (10 - 14) * 4, (5 - 14) * 4 },
    { (15 - 14) * 4, (10 - 14) * 4 },
    { (21 - 14) * 4, (5 - 14) * 4 },
    { (26 - 14) * 4, (10 - 14) * 4 },
    { (23 - 14) * 4, (15 - 14) * 4 },
    { (26 - 14) * 4, (20 - 14) * 4 },
    { (18 - 14) * 4, (25 - 14) * 4 },
    { (10 - 14) * 4, (25 - 14) * 4 },
    { (4 - 14) * 4, (20 - 14) * 4 },
    { (4 - 14) * 4, (17 - 14) * 4 },
    { (4 - 14) * 4, (14 - 14) * 4 },
    { (4 - 14) * 4, (10 - 14) * 4 },
    { (10 - 14) * 4, (5 - 14) * 4 },
};

struct point ASTEROID_C_BIG_POINTS[ASTEROID_NUM_POINTS] = {
    { (10 - 15) * 4, (5 - 14) * 4 },
    { (4 - 15) * 4, (10 - 14) * 4 },
    { (7 - 15) * 4, (15 - 14) * 4 },
    { (4 - 15) * 4, (19 - 14) * 4 },
    { (9 - 15) * 4, (24 - 14) * 4 },
    { (13 - 15) * 4, (21 - 14) * 4 },
    { (21 - 15) * 4, (24 - 14) * 4 },
    { (26 - 15) * 4, (17 - 14) * 4 },
    { (21 - 15) * 4, (13 - 14) * 4 },
    { (26 - 15) * 4, (9 - 14) * 4 },
    { (21 - 15) * 4, (5 - 14) * 4 },
    { (15 - 15) * 4, (7 - 14) * 4 },
    { (10 - 15) * 4, (5 - 14) * 4 },
};

