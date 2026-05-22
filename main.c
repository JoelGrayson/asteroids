#include "printf.h"
#include "uart.h"
#include "maths.h"
#include "assert.h"

int main() {
    uart_init();
    printf("Hi\n");
    printf("Bye 2 + 2 = %d\n", 2 + 2);
    assert(abs(-2) == 2);
    printf("Hi abs(-2) is %d\n", abs(-2));
}

