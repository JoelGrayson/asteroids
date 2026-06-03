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
#include "graphics/geometry.h"
#include "maths.h"
#include "buttons.h"
#include "timer.h"
#include "bullets.h"

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
// static unsigned long tickExplosion = 0; // Tick of explosion, used for rocket line erasure after set time of explosion.

static void configure_button_interrupts();
void collision_detection();
void update_mechanics_main();


int main() {
    uart_init();
    printf("Hello from main()\n");

    trig_init(3);
    gl_init(MONITOR_WIDTH, MONITOR_HEIGHT, FB_DOUBLEBUFFER);
    buttons_init();

    run_game();
}


// #define FPS 2
#define FPS 30
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
    /*rocket_explode_init();
    tickExplosion = timer_get_ticks(); // Registers the explosion tick, so we can progressively delete rocket explosion lines. */
}


static void run_one_frame() {
    gl_clear(GL_BLACK);

    /*for (int i = 0; i < CUR_NUM_ASTEROIDS; i++) {
        struct asteroid a = asteroids[i].ast;
        //printf("Displaying asteroid %d\n", i);
        struct point *points = get_points_of_asteroid(a);
        draw_points(points, ASTEROID_NUM_POINTS, a.mechanics.x, a.mechanics.y, GL_WHITE);
    }*/

    // Spawns new asteroid at regularly spaced intervals
    if(timer_get_ticks() - last_spawn_tick > SPAWN_INTERVAL_TICKS) {
        asteroid_spawn();
        last_spawn_tick = timer_get_ticks();
    }
    
    render_asteroids();
    render_rocket();
    draw_bullets();

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
    // Loops over each asteroid, and checks if it intersects with rocket.
    /*for(int i = 0; i < CUR_NUM_ASTEROIDS; i++) {
        struct asteroid ast = asteroids[i];
        struct point *ast_pts = get_points_of_asteroid(ast);
        double ast_xoff = ast.mechanics.x;
        double ast_yoff = ast.mechanics.y;
        for(int j = 0; j < get_num_asteroid_points() - 1; j++) {
            struct point p1 = { ast_pts[j].x+ast_xoff, ast_pts[j].y+ast_yoff };
            struct point p2 = { ast_pts[j+1].x+ast_xoff, ast_pts[j+1].y+ast_yoff };
            struct point *rocket_pts = get_points_of_rocket();

            struct mechanics rocket_mech = get_rocket_mechanics();
            double rocket_xoff = rocket_mech.x;
            double rocket_yoff = rocket_mech.y;
            for(int k = 0; k < get_num_rocket_points() - 1; k++) {
                struct point p3 = { rocket_pts[k].x+rocket_xoff, rocket_pts[k].y+rocket_yoff };
                struct point p4 = { rocket_pts[k+1].x+rocket_xoff, rocket_pts[k+1].y+rocket_yoff };
                if(lines_intersect(p1, p2, p3, p4)) printf("collision between rocket and asteroid detected!\n");
            }
        }
    }*/
    if(rocket_asteroid_collision()) printf("Rocket has collided with an asteroid!\n");
}

// Updates the position of all asteroids, rocket, and bullets using velocity and position
void update_mechanics_main() {
    asteroids_update_mechanics();
    rocket_update_mechanics();
    bullets_update_mechanics();
}
