#include "../libmango/assert.h"
#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "../graphics/draw_line.h"
#include "../rocket.h"
#include "../graphics/draw_asteroid.h"
#include "gl.h"
#include "fb.h"
#include "../constants.h"

void test_draw_asteroid();

int main() {
    uart_init();
    printf("Testing draw_line\n");
    test_draw_asteroid();
    printf("Done!\n"); //should never be called
}

void test_draw_asteroid() {
    gl_init(MONITOR_WIDTH, MONITOR_HEIGHT, FB_SINGLEBUFFER);

    gl_clear(GL_BLACK);

    gl_draw_string(600, 400, "Asteroid test", GL_WHITE);

    draw_asteroid(ASTEROID_A_SMALL, GL_WHITE);

    while (true); //actually show it
}

