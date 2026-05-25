#include "../libmango/assert.h"
#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "../graphics/draw_line.h"
#include "../rocket.h"
#include "../graphics/draw_asteroid.h"
#include "gl.h"
#include "fb.h"

void test_draw_asteroid();

int main() {
    uart_init();
    printf("Testing draw_line\n");
    test_draw_asteroid();
    printf("Done!\n"); //should never be called
}

void test_draw_asteroid() {
    gl_init(1280, 720, FB_SINGLEBUFFER);

    gl_clear(GL_BLACK); //yellow bg

    gl_draw_string(600, 400, "Asteroid test", GL_WHITE);

    draw_asteroid(ASTEROID_1_POINTS, GL_WHITE);

    while (true); //actually show it
}

