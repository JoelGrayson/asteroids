#include "printf.h"
#include "uart.h"

int main() {
    uart_init();
    printf("Testing maths %d\n", 1);
}

