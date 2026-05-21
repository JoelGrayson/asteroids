#include "printf.h"
#include "uart.h"

int main() {
    uart_init();
    printf("Hi 2+2=%d\n", 2+2);
}

