#include "buttons.h"
#include "gpio_interrupt.h"
#include "printf.h"

void onclick_thrust(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    printf("Thrust!\n");
    gpio_interrupt_clear(b->pin);
}
void onclick_fire(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    printf("Fire!\n");
    gpio_interrupt_clear(b->pin);
}
void onclick_left(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    printf("Left!\n");
    gpio_interrupt_clear(b->pin);
}
void onclick_right(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    printf("Right!\n");
    gpio_interrupt_clear(b->pin);
}
void onclick_teleport(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    printf("Teleport!\n");
    gpio_interrupt_clear(b->pin);
}
