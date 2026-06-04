#pragma once

#include "rocket.h"

#include "gpio.h"
#include "libmango/gpio_extra.h"
#include "interrupts.h"
#include "gpio_interrupt.h"
#include "printf.h"
#include "rocket.h"
#include "audio/sounds.h"
#include "start_game_screen.h"

#define NUM_BUTTONS 8

/** Sets up event listeners (button press and release) */
void buttons_init();

