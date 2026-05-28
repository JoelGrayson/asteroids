#include "buttons.h"
#include "gpio_interrupt.h"

void onclick_thrust(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    gpio_interrupt_clear(b->pin);
}
void onclick_fire(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    gpio_interrupt_clear(b->pin);
}
void onclick_left(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    gpio_interrupt_clear(b->pin);
}
void onclick_right(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    gpio_interrupt_clear(b->pin);
}
void onclick_teleport(void* aux_data) {
    // TODO!
    struct button *b = (struct button*)aux_data;
    gpio_interrupt_clear(b->pin);
}
