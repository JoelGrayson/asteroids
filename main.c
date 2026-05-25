#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "constants.h"
#include "fb.h"
#include "gl.h"
#include "asteroid.h"

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
    struct asteroid x = {
        .size = BIG,
        .type = A,
        .mechanics = { 100, 100, 0, 0, 0, 0, 0 }
    };
}

static void run_one_frame() {
    gl_clear(GL_BLACK);

    for (int i = 0; i < MAX_NUM_ASTEROIDS; i++) {
        
    }
    
    fb_swap_buffer(); //show the frame
}

