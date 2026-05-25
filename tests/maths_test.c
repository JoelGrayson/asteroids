#include "../libmango/assert.h"
#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "../maths.h"
#include "../rocket.h"

void test_max();
void test_factorial();
void test_sine();

int main() {
    uart_init();
    trig_init(3);
    trig_init(5);
    printf("Testing max()\n");
    test_max();
    printf("Testing factorial()\n");
    test_factorial();
    printf("Testing sine()\n");
    test_sine();
    printf("All tests passed!\n");
}

void test_max() {
    assert(max(1.0, 4.0) == 4.0);
    assert(max(-20.0, -3.0) == -3.0);
    assert(max(-20.0, 4.0) == 4.0);
    assert(max(-20.0, 0.0) == 0.0);
    assert(max(20.0, 0.0) == 20.0);
}

void test_factorial() {
    int fact = 1;
    assert(factorial(0) == fact);
    for(int i = 1; i < 20; i++) {
        fact *= i;
        assert(factorial(i) == fact);
    }
}

void test_sine() {
    printf("sine(1.571) = 0.%d\n", (int)(100000*sine(1.571)));
    printf("sine(0.785) = 0.%d\n", (int)(100000*sine(0.785)));
}
