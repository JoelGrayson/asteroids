#include "../libmango/assert.h"
#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "../gl_lib.h"
#include "../rocket.h"
#include "gl.h"
#include "fb.h"
#include "../gl_lib.h"

void test_draw_line();

int main() {
    uart_init();
    printf("Testing draw_line\n");
    test_draw_line();
    printf("Done!\n"); //should never be called
}

void test_draw_line() {
    gl_init(1280, 720, FB_SINGLEBUFFER);

    color_t yellow = gl_color(0xf9, 0xc4, 0x4d);
    gl_clear(yellow); //yellow bg

    gl_draw_string(600, 400, "HI THERE", GL_RED);
    
    // Test direction_of_most_movement == Y
    struct point p1 = { 10, 50 };
    struct point p2 = { 20, 10 };
    draw_line(p1, p2, GL_BLACK); //draw ↗
    p2.y = 90;
    draw_line(p1, p2, GL_BLUE); //draw ↘
    
    p1.x = 100;
    p1.y = 100;
    p2.x = 100 - 10;
    p2.y = 100 - 50;
    draw_line(p1, p2, GL_RED); //draw ↖

    p1.x = 100;
    p1.y = 100;
    p2.x = 100 - 10;
    p2.y = 100 + 50;
    draw_line(p1, p2, GL_PURPLE); //draw ↙


    
    while (true); //actually show it
}

