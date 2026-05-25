#include "../libmango/assert.h"
#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "../maths.h"
#include "../rocket.h"

void test_max();
void test_sine();

int main() {
    uart_init();
    trig_init(3);
    printf("Testing maths %d\n", 1);
    test_max();
    test_sine();
}

void test_max() {
    assert(max(1.0, 4.0) == 4.0);
    assert(max(-20.0, -3.0) == -3.0);
    assert(max(-20.0, 4.0) == 4.0);
    assert(max(-20.0, 0.0) == 0.0);
    assert(max(20.0, 0.0) == 20.0);
}

void test_sine() {
    printf("100*sine(1.571) = %d\n", (int)(100*sine(1.571)));
}
