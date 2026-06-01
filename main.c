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

// Array of buttons (pin + onclick interrupt handler function).
struct button buttons[NUM_BUTTONS] = {
    {GPIO_PC0, onclick_thrust},
    {GPIO_PD13, onclick_fire},
    {GPIO_PD10, onclick_left},
    {GPIO_PD11, onclick_right},
    {GPIO_PD15, onclick_teleport}
};



static void setup_game();
static void run_game();
static void run_one_frame();

// Array of pointers to asteroids. If no asteroid, it is NULL
static struct asteroid asteroids[MAX_NUM_ASTEROIDS];

// static struct bullet bullets[MAX_NUM_BULLETS];



static int frame = 0;
static unsigned long tickExplosion = 0; // Tick of explosion, used for rocket line erasure after set time of explosion.

static void configure_button_interrupts();
void collision_detection();
void update_mechanics();
void update_mechanics_main();


int main() {
    interrupts_init();
    uart_init();
    trig_init(3);
    printf("Hello from main()\n");
    gl_init(MONITOR_WIDTH, MONITOR_HEIGHT, FB_DOUBLEBUFFER);

    printf("Hello, welcome to Asteroids\n");
    
    configure_button_interrupts();
    run_game();
}

#define TEST_BUTTON GPIO_PB4

static void configure_button_interrupts() {
    gpio_interrupt_init();
    
    // Button handling interrupt setup:
    for (int i = 0; i < NUM_BUTTONS; i++) {
        // button click on negative edge, with debouncing.
        gpio_interrupt_config(buttons[i].pin, GPIO_INTERRUPT_NEGATIVE_EDGE, true);
        // assigns button handler function to its gpio pin interrupt response; passing in button itself as aux_data.
        gpio_interrupt_set_handler(buttons[i].pin, buttons[i].handler, &buttons[i]);
    }

    gpio_set_input(TEST_BUTTON);
    gpio_set_pullup(TEST_BUTTON);
    
    interrupts_global_enable();
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
    asteroids[0] = (struct asteroid) {
        .size = BIG,
        .type = A,
        .mechanics = { 100, 100, 0, 0, 0, 0, 0 }
    };
    asteroids[1] = (struct asteroid) {
        .size = BIG,
        .type = B,
        .mechanics = { 300, 100, 0, 0, 0, 0, 0 }
    };
    asteroids[2] = (struct asteroid) {
        .size = BIG,
        .type = C,
        .mechanics = { 500, 100, 0, 0, 0, 0, 0 }
    };

    asteroids[3] = (struct asteroid) {
        .size = MEDIUM,
        .type = A,
        .mechanics = { 100, 300, 0, 0, 0, 0, 0 }
    };
    asteroids[4] = (struct asteroid) {
        .size = MEDIUM,
        .type = B,
        .mechanics = { 300, 300, 0, 0, 0, 0, 0 }
    };
    asteroids[5] = (struct asteroid) {
        .size = MEDIUM,
        .type = C,
        .mechanics = { 500, 300, 0, 0, 0, 0, 0 }
    };

    asteroids[6] = (struct asteroid) {
        .size = SMALL,
        .type = A,
        .mechanics = { 100, 500, 0, 0, 0, 0, 0 }
    };
    asteroids[7] = (struct asteroid) {
        .size = SMALL,
        .type = B,
        .mechanics = { 300, 500, 0, 0, 0, 0, 0 }
    };
    asteroids[8] = (struct asteroid) {
        .size = SMALL,
        .type = C,
        .mechanics = { 500, 500, 0, 0, 0, 0, 0 }
    };

    rocket_explode_init();
    tickExplosion = timer_get_ticks(); // Registers the explosion tick, so we can progressively delete rocket explosion lines.
}

void onclick_thrust(void* aux_data) {
    struct button *b = (struct button*)aux_data;
    printf("Button pressed: thrust\n");
    
    
    
    
    gpio_interrupt_clear(b->pin);
}

void onclick_fire(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    printf("Fire!\n");
    gpio_interrupt_clear(b->pin);
}
void onclick_left(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    printf("Left!\n");
    gpio_interrupt_clear(b->pin);
}
void onclick_right(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    printf("Right!\n");
    gpio_interrupt_clear(b->pin);
}
void onclick_teleport(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    printf("Teleport!\n");
    gpio_interrupt_clear(b->pin);
}



static void run_one_frame() {
    gl_clear(GL_BLACK);

    for (int i = 0; i < 9; i++) {
        struct asteroid a = asteroids[i];
        // printf("Displaying asteroid %d\n", i);
        struct point *points = get_points_of_asteroid(a);
        draw_points(points, ASTEROID_NUM_POINTS, a.mechanics.x, a.mechanics.y, GL_WHITE);
    }

    // draw_points(rotate_points(ROCKET_POINTS, rocket_mechanics.rotation), ROCKET_NUM_POINTS, 500, 400, GL_WHITE);
    draw_points(ROCKET_POINTS, ROCKET_NUM_POINTS, 500, 400, GL_WHITE);
    /*printf("rocket num points is: %d\n", ROCKET_NUM_POINTS);
    printf("__rocket num points is: %d\n", __ROCKET_NUM_POINTS);
    printf("rocket exploded sides num points is: %d\n", ROCKET_EXPLODED_SIDES_NUM_POINTS);*/

    // Draws all rocket exploded sides (- the number of despawned exploded sides).
    if(tickExplosion != 0) {
        unsigned long sides_to_despawn = timer_get_ticks()-tickExplosion;
        sides_to_despawn /= TICKS_EXPLOSION_DISAPPEAR;
        for (unsigned long i = 0; i < __ROCKET_NUM_POINTS-1-sides_to_despawn; i++) { // draws exploded sides (if rocket has exploded).
            draw_points(ROCKET_EXPLODED_POINTS[i], ROCKET_EXPLODED_SIDES_NUM_POINTS, 500, 400, GL_WHITE);
        }
    }
    collision_detection();
    rocket_explode_update();
    update_mechanics_main();
    fb_swap_buffer(); //show the frame
    
    printf("Button: %d\n", gpio_read(TEST_BUTTON));
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
    // for ()
}
