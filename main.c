#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "constants.h"
#include "interrupts.h"
#include "gpio_interrupt.h"

#include "asteroid.h"
// #include "bullet.h"
#include "rocket.h"
#include "gpio.h"
#include "libmango/gpio_extra.h"

#include "fb.h"
#include "gl.h"
#include "graphics/draw_points.h"
#include "maths.h"
#include "buttons.h"
#include "timer.h"

#define TICKS_EXPLOSION_DISAPPEAR 2000000*TICKS_PER_USEC


static void setup_game();
static void run_game();
static void run_one_frame();

// Array of pointers to asteroids. If no asteroid, it is NULL
static struct asteroid asteroids[MAX_NUM_ASTEROIDS];
static int CUR_NUM_ASTEROIDS = 9; // Number of asteroids to actively draw, update, and check collisions for.

// static struct bullet bullets[MAX_NUM_BULLETS];



static int frame = 0;
// static unsigned long tickExplosion = 0; // Tick of explosion, used for rocket line erasure after set time of explosion.

static void configure_button_interrupts();
void collision_detection();
void update_mechanics_main();


int main() {
    uart_init();
    printf("Hello from main()\n");

    buttons_init();
    trig_init(3);
    gl_init(MONITOR_WIDTH, MONITOR_HEIGHT, FB_DOUBLEBUFFER);

    run_game();
}

static void run_game() {
    printf("Starting game\n");
    setup_game();
    
    while (true) {
        run_one_frame();
        frame++;
    }
}

static void setup_game() {
    for(int i = 0; i < CUR_NUM_ASTEROIDS; i++) {
        asteroid_respawn(&asteroids[i]);
    }
    /*rocket_explode_init();
    tickExplosion = timer_get_ticks(); // Registers the explosion tick, so we can progressively delete rocket explosion lines. */
}


static void run_one_frame() {
    gl_clear(GL_BLACK);

    for (int i = 0; i < CUR_NUM_ASTEROIDS; i++) {
        struct asteroid a = asteroids[i];
        // printf("Displaying asteroid %d\n", i);
        struct point *points = get_points_of_asteroid(a);
        draw_points(points, ASTEROID_NUM_POINTS, a.mechanics.x, a.mechanics.y, GL_WHITE);
    }

    render_rocket();

    // Draws all rocket exploded sides (- the number of despawned exploded sides).
    // if(tickExplosion != 0) {
    //     unsigned long sides_to_despawn = timer_get_ticks()-tickExplosion;
    //     sides_to_despawn /= TICKS_EXPLOSION_DISAPPEAR;
    //     for (unsigned long i = 0; i < __ROCKET_NUM_POINTS-1-sides_to_despawn; i++) { // draws exploded sides (if rocket has exploded).
    //         draw_points(ROCKET_EXPLODED_POINTS[i], ROCKET_EXPLODED_SIDES_NUM_POINTS, rocket_mechanics.x, rocket_mechanics.y, GL_WHITE);
    //     }
    // }

    collision_detection();
    // rocket_explode_update();
    update_mechanics_main();
    fb_swap_buffer(); //show the frame
    
    // printf("Button: %d\n", gpio_read(TEST_BUTTON));
}


void collision_detection() {
    // for each asteroid
    //      for each bullet
    //            if asteroid touching bullet
    // 


/*
Loop over each bullet:
    check if touching asteroid
    

check if rocket touching asteroid

*/
}

// Updates the position of all asteroids, rocket, and bullets using velocity and position
void update_mechanics_main() {
    asteroids_update_mechanics(asteroids, CUR_NUM_ASTEROIDS);
    rocket_update_mechanics();
}
