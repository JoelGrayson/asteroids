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
}

void test_draw_line() {
    gl_init(1280, 720, FB_SINGLEBUFFER);
    color_t yellow = gl_color(0xf9, 0xc4, 0x4d);
    gl_clear(yellow); //yellow bg

    struct point p1 = { 10, 10 };
    struct point p2 = { 20, 20 };
    draw_line(p1, p2, GL_BLACK);
}

