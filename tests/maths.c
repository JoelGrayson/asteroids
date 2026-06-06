// #include "../libmango/assert.h"
#include "assert.h"
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
    trig_init(5);
    printf("Testing maths %d\n", 1);
    test_max();
    test_factorial();
    test_sine();
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
    for(int i = 1; i < 100; i++) {
        fact *= i;
        assert(factorial(i) == fact);
    }
}

void test_sine() {
    printf("100*sine(1.571) = %d\n", (int)(100*sine(1.571)));
}
