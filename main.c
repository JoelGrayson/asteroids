#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "constants.h"

#include "asteroid.h"

#include "fb.h"
#include "gl.h"
#include "graphics/draw_points.h"

static void setup_game();
static void run_game();
static void run_one_frame();

static struct asteroid asteroids[MAX_NUM_ASTEROIDS];

static int frame = 0;

int main() {
    uart_init();
    gl_init(MONITOR_WIDTH, MONITOR_HEIGHT, FB_DOUBLEBUFFER);

    run_game();
}

static void run_game() {
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
        .mechanics = { 200, 100, 0, 0, 0, 0, 0 }
    };
    asteroids[2] = (struct asteroid) {
        .size = BIG,
        .type = C,
        .mechanics = { 300, 100, 0, 0, 0, 0, 0 }
    };

    asteroids[3] = (struct asteroid) {
        .size = MEDIUM,
        .type = A,
        .mechanics = { 200, 100, 0, 0, 0, 0, 0 }
    };
    asteroids[4] = (struct asteroid) {
        .size = MEDIUM,
        .type = B,
        .mechanics = { 200, 200, 0, 0, 0, 0, 0 }
    };
    asteroids[5] = (struct asteroid) {
        .size = MEDIUM,
        .type = C,
        .mechanics = { 200, 300, 0, 0, 0, 0, 0 }
    };

    asteroids[6] = (struct asteroid) {
        .size = SMALL,
        .type = A,
        .mechanics = { 300, 100, 0, 0, 0, 0, 0 }
    };
    asteroids[7] = (struct asteroid) {
        .size = SMALL,
        .type = B,
        .mechanics = { 300, 200, 0, 0, 0, 0, 0 }
    };
    asteroids[8] = (struct asteroid) {
        .size = SMALL,
        .type = C,
        .mechanics = { 300, 300, 0, 0, 0, 0, 0 }
    };

}

static void run_one_frame() {
    gl_clear(GL_BLACK);

    for (int i = 0; i < 9; i++) {
        struct asteroid a = asteroids[i];
        struct point *points = get_points_of_asteroid(a);
        draw_points(points, GL_WHITE);
    }
    
    fb_swap_buffer(); //show the frame
}

