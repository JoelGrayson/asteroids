#include "../libmango/assert.h"
#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "../maths.h"
#include "../rocket.h"

void test_max();

int main() {
    uart_init();
    printf("Testing maths\n");
}

void test_max() {
    assert(max(1.0, 4.0) == 4.0);
    assert(max(-20.0, -3.0) == -3.0);
    assert(max(-20.0, 4.0) == 4.0);
    assert(max(-20.0, 0.0) == 0.0);
    assert(max(20.0, 0.0) == 20.0);
}

