#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "constants.h"
#include "interrupts.h"
#include "gpio_interrupt.h"

#include "asteroid.h"
#include "rocket.h"
#include "gpio.h"
#include "libmango/gpio_extra.h"

#include "fb.h"
#include "gl.h"
#include "graphics/draw_points.h"
#include "graphics/geometry.h"
#include "maths.h"
#include "buttons.h"
#include "timer.h"
#include "bullets.h"
#include "audio/sounds.h"
#include "fps.h"
#include "score_and_lives.h"

#define TICKS_EXPLOSION_DISAPPEAR 2000000*TICKS_PER_USEC

// Variable which controls the spawning interval of asteroids
static unsigned long SPAWN_INTERVAL_TICKS = 500000*TICKS_PER_USEC;
// Variable which tracks the last tick an asteroid was spawned
static unsigned long last_spawn_tick;

static void setup_game();
static void run_game();
static void run_one_frame();

static int CUR_NUM_ASTEROIDS = 9; // Number of asteroids to actively draw, update, and check collisions for.


static int frame = 0;

static void configure_button_interrupts();
void collision_detection();
void update_mechanics_main();


int main() {
    uart_init();
    printf("Hello from main()\n");

    trig_init(3);
    gl_init(MONITOR_WIDTH, MONITOR_HEIGHT, FB_DOUBLEBUFFER);
    sounds_init();
    buttons_init();
    score_and_lives_init();

    run_game();
}

const long min_time_per_frame = 24*1000*1000/FPS;

static void run_game() {
    printf("Starting game\n");
    setup_game();
    printf("Setup done\n");
    

    long ticks_at_start_of_loop = timer_get_ticks();
    while (true) {
        run_one_frame();
        frame++;

        while (timer_get_ticks() - ticks_at_start_of_loop < min_time_per_frame)
            ; // SPIN!

        ticks_at_start_of_loop = timer_get_ticks();
    }
}

static void setup_game() {
    for(int i = 0; i < CUR_NUM_ASTEROIDS; i++) {
        asteroid_spawn();
    }
    last_spawn_tick = timer_get_ticks();
}


static void run_one_frame() {
    // Clear frame to blank black frame
    gl_clear(GL_BLACK);

    // Spawns new asteroid at regularly spaced intervals
    if(timer_get_ticks() - last_spawn_tick > SPAWN_INTERVAL_TICKS) {
        asteroid_spawn();
        last_spawn_tick = timer_get_ticks();
    }
    
    render_asteroids();
    render_rocket();
    render_score_and_lives();
    draw_bullets();


    collision_detection();
    update_mechanics_main();
    fb_swap_buffer(); //show the frame
}


void collision_detection() {
    bullets_asteroid_collision();
    if(rocket_asteroid_collision()) {
        rocket_explode();
        decrease_lives();
        render_score_and_lives();
    }
}

// Updates the position of all asteroids, rocket, and bullets using velocity and position
void update_mechanics_main() {
    asteroids_update_mechanics();
    rocket_update_mechanics();
    bullets_update_mechanics();
}
