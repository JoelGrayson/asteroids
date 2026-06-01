#pragma once
#include "gpio.h"

typedef void (*click_fn_t)(void* aux_data);

#define NUM_BUTTONS 5

struct button {
    gpio_id_t pin;
    click_fn_t handler;
};

void onclick_thrust(void* aux_data);
void onclick_fire(void* aux_data);
void onclick_left(void* aux_data);
void onclick_right(void* aux_data);
void onclick_hyperspace(void* aux_data);

