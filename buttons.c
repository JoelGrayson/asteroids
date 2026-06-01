#include "buttons.h"

typedef void (*click_fn_t)(void* aux_data);

const gpio_id_t ROTATE_LEFT_BUTTON = GPIO_PB1;
const gpio_id_t ROTATE_RIGHT_BUTTON = GPIO_PB0;
const gpio_id_t HYPERSPACE_BUTTON = GPIO_PB4;
const gpio_id_t THRUST_BUTTON = GPIO_PB3;
const gpio_id_t FIRE_BUTTON = GPIO_PB2;

const gpio_id_t buttons[NUM_BUTTONS] = {
    ROTATE_LEFT_BUTTON,
    ROTATE_RIGHT_BUTTON,
    HYPERSPACE_BUTTON,
    THRUST_BUTTON,
    FIRE_BUTTON
};

// Rotate left and right have pressed and released listeners. Hyperspace, thrust, and fire only have pressed listeners
void rotate_left_pressed_listener();
void rotate_left_released_listener();
void rotate_right_pressed_listener();
void rotate_right_released_listener();
void hyperspace_pressed_listener();
void thrust_pressed_listener();
void fire_pressed_listener();

void buttons_init() {
    gpio_interrupt_init();
    
    // All are pull up so no resistors are needed in the circuit
    for (int i = 0; i < NUM_BUTTONS; i++) {
        gpio_set_pullup(buttons[i]);
    }

    gpio_interrupt_config();

}


// Rotate left and right have pressed and released listeners. Hyperspace, thrust, and fire only have pressed listeners
void rotate_left_pressed_listener();
void rotate_left_released_listener();
void rotate_right_pressed_listener();
void rotate_right_released_listener();
void hyperspace_pressed_listener();
void thrust_pressed_listener();
void fire_pressed_listener();


