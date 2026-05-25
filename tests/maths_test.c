#include "../libmango/assert.h"
#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "../maths.h"
#include "../rocket.h"

void test_max();
void test_round();
void test_sine();

int main() {
    uart_init();
    trig_init(3);
    printf("Testing math\n");
    test_max();
    test_round();
    test_sine();
    printf("All tests passed!\n");
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

void test_sine() {
    printf("100*sine(1.571) = %d\n", (int)(100*sine(1.571)));
}
