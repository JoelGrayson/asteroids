#pragma once

#include "rocket.h"

#include "gpio.h"
#include "libmango/gpio_extra.h"
#include "interrupts.h"
#include "gpio_interrupt.h"

#define NUM_BUTTONS 5

/** Sets up event listeners (button press and release) */
void buttons_init();

