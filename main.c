#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "constants.h"
#include "gpio.h"
#include "libmango/gpio_extra.h"
#include "interrupts.h"
#include "gpio_interrupt.h"
#include "fb.h"
#include "gl.h"

#include "graphics/draw_points.h"
#include "graphics/geometry.h"

#include "asteroid.h"
#include "rocket.h"
#include "maths.h"
#include "buttons.h"
#include "timer.h"
#include "bullets.h"
#include "audio/sounds.h"
#include "fps.h"
#include "score_and_lives.h"
#include "saucer.h"

static void one_time_setup();
static void run_game();
static void setup_game();
static void run_one_frame();
static void loop(long frame);
static void collision_detection();

int main() {
    one_time_setup();
    run_game();
}

// For the inits
static void one_time_setup() {
    uart_init();
    printf("One time setup\n");
    trig_init(3);
    gl_init(MONITOR_WIDTH, MONITOR_HEIGHT, FB_DOUBLEBUFFER);
    sounds_init();
    buttons_init();
    score_and_lives_init();
}

static void run_game() {
    // Setup
    setup_game();

    // Loop surrounded by checks to throttle fps if necessary
    long ticks_at_start_of_loop = timer_get_ticks();
    long frame = 0;
    while (true) {
        loop(frame);

        // If the frame finishes super fast, throttle it to FPS
        const long min_time_per_frame = 24*1000*1000/FPS;
        while (timer_get_ticks() - ticks_at_start_of_loop < min_time_per_frame)
            ; // SPIN!
        
        ticks_at_start_of_loop = timer_get_ticks();
        frame++;
    }
}

static void setup_game() {
    setup_asteroids();
    setup_score_and_lives();
    setup_rocket();
}

// Runs one frame of the game
static void loop(long frame) {
    // Clear frame to blank black frame
    gl_clear(GL_BLACK);
    
    loop_asteroids(frame);
    loop_rocket(frame);
    loop_score_and_lives(frame);
    loop_bullets(frame);
    loop_saucer(frame);

    collision_detection();
    
    // Show the frame
    fb_swap_buffer();
}


void collision_detection() {
    bullets_asteroid_collision();
    if(rocket_asteroid_collision()) {
        rocket_explode();
        decrease_lives();
    }
}

