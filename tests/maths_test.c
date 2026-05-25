#include "../libmango/assert.h"
#include "printf.h"
#include "uart.h"
#include "gl.h"
#include <stdbool.h>
#include "../maths.h"
#include "../rocket.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void test_max();
void test_factorial();
void test_sine();
void test_draw_circle();
void test_sgn(); //the other "sign" haha

int main() {
    uart_init();
    gl_init(SCREEN_WIDTH, SCREEN_HEIGHT, FB_SINGLEBUFFER);
    gl_clear(GL_BLACK);

    printf("maths_test\n");
    trig_init(4);
    printf("Testing max()\n");
    test_max();
    printf("Testing factorial()\n");
    test_factorial();
    /*printf("Testing sine()\n");
    test_sine();*/
    printf("Testing draw circle using sines and cosines!");
    test_draw_circle();
    printf("All tests passed!\n");
    while(1) { /* SPIN! */ }
}

void test_max() {
    assert(max(1.0, 4.0) == 4.0);
    assert(max(-20.0, -3.0) == -3.0);
    assert(max(-20.0, 4.0) == 4.0);
    assert(max(-20.0, 0.0) == 0.0);
    assert(max(20.0, 0.0) == 20.0);
    printf("test_max passes\n");
}

void test_round() {
    assert(round(1.0) == 1);
    assert(round(-1.0) == -1);
    assert(round(-4.4) == -4);
    assert(round(4.5) == 5);
    assert(round(99.9) == 100);
    assert(round(99.3) == 99);
    printf("test_round passes\n");
}

void test_factorial() {
    double fact = 1;
    assert(factorial(0) == fact);
    for(int i = 1; i < 20; i++) {
        fact *= i;
        assert(factorial(i) == fact);
    }
}

void test_sine() {
    int res = 100*sine(1.571);
    assert(res == 99);

    res = 100*cosine(1.571);
    assert(res == 0);

    res = 100*sine(0.785);
    assert(res == 70);

    res = 100*sine(-1.571);
    assert(res == -100);

    res = 100*cosine(2.356);
    assert(res == -70);
}

void test_draw_circle() {
    int x_center = SCREEN_WIDTH / 2;
    int y_center = SCREEN_HEIGHT / 2;
    double radius = 100;
    int circumference = (int)(2 * PI * radius) + 1;
    int num_angles = round((double)(circumference)/2);
    double angle_it = 2*PI / (double)num_angles;
    
    double angle = 0;
    for(int i = 0; i < num_angles; i++) {
        int x = x_center + round(radius*cosine(angle));
        int y = y_center + round(radius*sine(angle));
        gl_draw_pixel(x, y, GL_WHITE);
        angle += angle_it;
    }
}
