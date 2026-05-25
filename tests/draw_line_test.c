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


static void test_draw_strings(void) {
    gl_init(1280, 720, FB_SINGLEBUFFER); // init double buffer
    
    gl_clear(gl_color(0xf9, 0xc4, 0x4d));
    
    int x = 20;
    int y = 20;
    int delta_x = 70;
    int delta_y = 20;
    while (true) {
        gl_draw_string(x, y, "E", GL_RED);

        if (x > 1200) {
            x = 20;
            y += delta_y;
        } else {
            x += delta_x;
        }
    }
}


int main() {
    uart_init();
    printf("Testing draw_line\n");
    // test_draw_line();
    test_draw_strings();
}

void test_draw_line() {
    gl_init(1280, 720, FB_SINGLEBUFFER);
    color_t yellow = gl_color(0xf9, 0xc4, 0x4d);
    gl_clear(yellow); //yellow bg

    // struct point p1 = { 10, 10 };
    // struct point p2 = { 20, 20 };
    // draw_line(p1, p2, GL_BLACK);    
    gl_draw_string(100, 100, "HI THERE", GL_RED);
}
