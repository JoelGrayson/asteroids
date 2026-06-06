#include "assert.h"
#include "printf.h"
#include "uart.h"
#include <stdbool.h>
#include "../atan2.h"
#include "../angle_from.h"

void test_atan2();

int main() {
    uart_init();
    printf("Testing atan2 %d\n", 1);
    test_atan2();
}

void test_atan2() {
    printf("atan2(20, 10) = %d / 100\n", (int)(atan2(20, 10) * 100));
    printf("atan2(20, 0) = %d / 100 (should be 314/100)\n", (int)(atan2(20, 0) * 100));
    // (1, 0) is pi/2 but should map to pi
    // (20, 10) is 110/100 but should be ~pi*3/4 = 2.3

    printf("angle_from((0, 0), (10, 10)) should be 3/4*pi=2.35 and is %d / 100\n", (int)(angle_from((struct point){0,0},(struct point){10,10})*100));

    printf("angle_from((20,20), (18, 10)) should be -0.1 or 6.0 and is %d / 100\n", (int)(angle_from((struct point){20,20},(struct point){18,10})*100));
}

