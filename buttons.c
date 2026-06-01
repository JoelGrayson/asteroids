#include "buttons.h"

typedef void (*click_fn_t)(void* aux_data);

const gpio_id_t ROTATE_LEFT_BUTTON_PRESS = GPIO_PB0;
const gpio_id_t ROTATE_LEFT_BUTTON_RELEASE = GPIO_PB0;
const gpio_id_t ROTATE_RIGHT_BUTTON_PRESS = GPIO_PB4; //falling edge
const gpio_id_t ROTATE_RIGHT_BUTTON_RELEASE = GPIO_PB3; //rising edge
const gpio_id_t HYPERSPACE_BUTTON = GPIO_PB0;
const gpio_id_t THRUST_BUTTON = GPIO_PB0;
const gpio_id_t FIRE_BUTTON = GPIO_PB0;

const gpio_id_t buttons[NUM_BUTTONS] = {
    ROTATE_LEFT_BUTTON_PRESS,
    ROTATE_LEFT_BUTTON_RELEASE,
    ROTATE_RIGHT_BUTTON_PRESS,
    ROTATE_RIGHT_BUTTON_RELEASE,
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
    interrupts_init();
    gpio_interrupt_init();
    
    // All are pull up so no resistors are needed in the circuit
    for (int i = 0; i < NUM_BUTTONS; i++) {
        gpio_set_pullup(buttons[i]);
    }

    gpio_interrupt_config(ROTATE_LEFT_BUTTON_PRESS, GPIO_INTERRUPT_NEGATIVE_EDGE, true);
    gpio_interrupt_set_handler(ROTATE_LEFT_BUTTON_PRESS, rotate_left_pressed_listener, NULL);

    gpio_interrupt_config(ROTATE_LEFT_BUTTON_RELEASE, GPIO_INTERRUPT_POSITIVE_EDGE, true);
    gpio_interrupt_set_handler(ROTATE_LEFT_BUTTON_RELEASE, rotate_left_released_listener, NULL);
    
    gpio_interrupt_config(ROTATE_RIGHT_BUTTON_PRESS, GPIO_INTERRUPT_NEGATIVE_EDGE, true);
    gpio_interrupt_set_handler(ROTATE_RIGHT_BUTTON_PRESS, rotate_right_pressed_listener, NULL);
    
    gpio_interrupt_config(ROTATE_RIGHT_BUTTON_RELEASE, GPIO_INTERRUPT_POSITIVE_EDGE, true);
    gpio_interrupt_set_handler(ROTATE_RIGHT_BUTTON_RELEASE, rotate_right_released_listener, NULL);
    
    gpio_interrupt_config(HYPERSPACE_BUTTON, GPIO_INTERRUPT_NEGATIVE_EDGE, true);
    gpio_interrupt_set_handler(HYPERSPACE_BUTTON, hyperspace_pressed_listener, NULL);
    
    gpio_interrupt_config(THRUST_BUTTON, GPIO_INTERRUPT_NEGATIVE_EDGE, true);
    gpio_interrupt_set_handler(THRUST_BUTTON, thrust_pressed_listener, NULL);
    
    gpio_interrupt_config(FIRE_BUTTON, GPIO_INTERRUPT_NEGATIVE_EDGE, true);
    gpio_interrupt_set_handler(FIRE_BUTTON, fire_pressed_listener, NULL);


    // Knife switch
    interrupts_global_enable();
}


// Rotate left and right have pressed and released listeners. Hyperspace, thrust, and fire only have pressed listeners
void rotate_left_pressed_listener() {

}

void rotate_left_released_listener() {

}

void rotate_right_pressed_listener() {
    printf("Button pressed: right\n");
    rocket_rotate_right();

    gpio_interrupt_clear(ROTATE_RIGHT_BUTTON_PRESS);
}

void rotate_right_released_listener() {
    printf("Button released: right\n");
    rocket_rotate_right();

    gpio_interrupt_clear(ROTATE_RIGHT_BUTTON_RELEASE);
}

void hyperspace_pressed_listener() {
    printf("Button pressed: hyperspace\n");
    rocket_hyperspace();

    gpio_interrupt_clear(HYPERSPACE_BUTTON);
}

void thrust_pressed_listener() {
    printf("Button pressed: thrust\n");
    rocket_thrust();

    gpio_interrupt_clear(THRUST_BUTTON);
}

void fire_pressed_listener() {
    printf("Button pressed: fire\n");
    rocket_fire();

    gpio_interrupt_clear(FIRE_BUTTON);
}

