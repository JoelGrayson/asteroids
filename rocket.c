#include <stdbool.h>
#include "rocket.h"
#include "graphics/geometry.h"
#include "graphics/rotate_vector.h"
#include "maths.h"
#include "timer.h"
#include "rand.h"
#include "bullets.h"
#include "asteroid.h"
#include "audio/sounds.h"

#define ROCKET_DECELERATION 0.22

// Sound timing tracking variables for I2S non-blocking play:
unsigned long last_fire_sound_tick;
unsigned long last_thrust_sound_tick;

extern unsigned long last_explosion_sound_tick;

// Points of a rocket facing north
struct point ROCKET_POINTS_TEMPLATE[ROCKET_NUM_POINTS] = {
    {   0, -20 }, //1
    //    /
    { -13,  20 }, //2
    // 
    { -10,  11 }, //3
    {  10,  11 }, //4
    {  13,  20 }, //5
    {   0, -20 }  //6 (same as 1)
};

// Starts off as the same as ROCKET_POINTS_TEMPLATE
static struct point rotated_rocket_points[ROCKET_NUM_POINTS] = {
    {   0, -20 },
    { -13,  20 },
    { -10,  11 },
    {  10,  11 },
    {  13,  20 },
    {   0, -20 }
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


static bool is_rotating_left = false;
static bool is_rotating_right = false;


void rocket_rotate_left_press() {
    is_rotating_left = true;
}

void rocket_rotate_left_release() {
    is_rotating_left = false;
}

void rocket_rotate_right_press() {
    is_rotating_right = true;
}

void rocket_rotate_right_release() {
    is_rotating_right = false;
}



/** Creates a bullet */
void rocket_fire() {
    unsigned long present_tick = timer_get_ticks();
    if(present_tick-last_fire_sound_tick <= FIRE_SOUND_TICK_DURATION) return; // Refuse to fire faster than sound play!
    struct mechanics *mech = &rocket_mechanics;
    // Calculates direction bullet should be heading in
    struct vector dir = {0, -1.5};
    rotate_vector(&dir, mech->rotation);

    // Instantiates bullet in said direction.
    struct mechanics mech_bullet = {
        .x = mech->x,
        .y = mech->y,
        .vx = 10*dir.x,
        .vy = 10*dir.y,
        .ax = 0,
        .ay = 0,
        .rotation = mech->rotation
    };
    new_bullet(mech_bullet, ROCKET);
    present_tick = timer_get_ticks();
    if(present_tick-last_fire_sound_tick > FIRE_SOUND_TICK_DURATION && present_tick-last_thrust_sound_tick > THRUST_SOUND_TICK_DURATION && present_tick-last_explosion_sound_tick > EXPLOSION_SOUND_TICK_DURATION) {
        play_fire();
        last_fire_sound_tick = timer_get_ticks();
    }
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
        ROCKET_POINTS_TEMPLATE,
        ROCKET_NUM_POINTS,
        rocket_mechanics.rotation
    );
}

// Draws the rocket
void render_rocket() {
    if (rocket_is_exploding) {
        // gl_draw_pixel(rocket_mechanics.x, rocket_mechanics.y, GL_WHITE);

        // Draw three lines: /, \, and _
        render_explosion((struct point){ .x = rocket_mechanics.x, .y = rocket_mechanics.y }, num_frames_after_rocket_exploded);

        if (num_frames_after_rocket_exploded >= NUM_FRAMES_OF_EXPLOSION) {
            // Explosion over
            rocket_is_exploding = 0;
            num_frames_after_rocket_exploded = 0;
        }
    } else {
        // Normal rocket
        draw_points(rotated_rocket_points, ROCKET_NUM_POINTS, rocket_mechanics.x, rocket_mechanics.y, GL_WHITE);
    }
}


void rocket_explode() {
    rocket_is_exploding = true;
    num_frames_after_rocket_exploded = 0;
}

struct point *get_points_of_rocket() {
    return rotated_rocket_points;
}

int get_num_rocket_points() {
    return ROCKET_NUM_POINTS;
}

bool rocket_asteroid_collision() {
    // Tries to find an asteroid which has collided with our rocket:
    struct asteroid_list_t *to_explode = get_asteroid_collision(rocket_mechanics, rotated_rocket_points, ROCKET_NUM_POINTS);
    // If an asteroid has collided with our rocket, remove it (explode it!) and then return true (there has been a collision)
    if(to_explode != NULL) {
        asteroid_explode(to_explode);
        return true;
    }
    return false; // No asteroid collision, return false.
}

void rocket_update_mechanics() {
    struct mechanics *mech = &rocket_mechanics;

    if (is_rotating_left) {
        mech->rotation--;
    } else if (is_rotating_right) {
        mech->rotation++;
    }
    
    
    if(rocket_is_thrusting) {
        // Rocket thrusts with magnitude 1.5 acceleration in the direction of its cone
        struct vector dir = {0, -1.5};
        rotate_vector(&dir, mech->rotation);
        mech->ax = dir.x;
        mech->ay = dir.y;
        unsigned long present_tick = timer_get_ticks();
        if(present_tick-last_fire_sound_tick > FIRE_SOUND_TICK_DURATION && present_tick-last_thrust_sound_tick > THRUST_SOUND_TICK_DURATION && present_tick-last_explosion_sound_tick > EXPLOSION_SOUND_TICK_DURATION) {
            play_thrust();
            last_thrust_sound_tick = timer_get_ticks();
        }
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

    if (rocket_is_exploding) {
        num_frames_after_rocket_exploded++;
    }
}


