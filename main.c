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
#include "start_game_screen.h"
#include "game_over_screen.h"
#include "collision_detection.h"
#include "frame.h"
#include "game_manager_state.h"

#define NUM_FRAMES_TIL_SPEEDUP 24*40

static void game_manager();
static void one_time_setup();
static void run_game();
static void setup_game();
static void run_one_frame();
static void loop();

extern int num_frames_between_spawn;
static bool beat1;

enum game_manager_state game_manager_state;

int main() {
    one_time_setup();
    game_manager();
}

static void game_manager() {
    while (true) {
        start_game_screen(); // Start game screen!
        run_game(); // Game run screen!
        while(game_manager_state == GAME_OVER_SCREEN) {
            game_over_screen();
        }
    }
}

// For the inits
static void one_time_setup() {
    uart_init();
    printf("One time setup\n");
    trig_init(3);
    gl_init(MONITOR_WIDTH, MONITOR_HEIGHT, FB_DOUBLEBUFFER);
    game_manager_state = START_GAME_SCREEN; // Always at start screen upon initial boot!
    sounds_init();
    buttons_init();
    score_and_lives_init();
    reset_highscores();
}

static void run_game() {
    // Setup
    setup_game();
    beat1 = true;

    // Loop surrounded by checks to throttle fps if necessary
    long ticks_at_start_of_loop = timer_get_ticks();
    frame = 0;
    while (true) {
        loop(frame);

        // If the frame finishes super fast, throttle it to FPS
        const long min_time_per_frame = 24*1000*1000/FPS;
        while (timer_get_ticks() - ticks_at_start_of_loop < min_time_per_frame)
            ; // SPIN!
        
        // Game over
        if (get_num_lives() <= 0 && !rocket_is_exploding) {
            game_manager_state = GAME_OVER_SCREEN;
            setup_gameover(); // Sets up game over screen.
            return;
        }

        // Plays game beat every num_frames_between_spawn-th frame!
        if(frame % num_frames_between_spawn == 0) {
            if(beat1) { 
                play_beat1();
                beat1 = false;
            } else {
                play_beat2();
                beat1 = true;
            }
        }

        // Game very slowly speeds up in difficulty and song beat as num_frames_between_spawn decreases:
        if(frame % NUM_FRAMES_TIL_SPEEDUP == 0) {
            if(num_frames_between_spawn > 4) num_frames_between_spawn--;
        }
            
        ticks_at_start_of_loop = timer_get_ticks();
        frame++;
    }
}

static void setup_game() {
    num_frames_between_spawn = 2 * FPS; // Sets number of seconds between asteroid spawns to default at 2.
    setup_asteroids();
    setup_score_and_lives();
    setup_rocket();
    setup_saucer();
}

// Runs one frame of the game
static void loop() {
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
    // Play sounds!
    sounds_play();
}

