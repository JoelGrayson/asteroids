/* File: extension.c
 * -----------------
 * ***** File full of my extension work in implementing pull-up/down/none states for the Mango Pi gpio pins! *****
 */
#include <stdbool.h>
#include "gpio_extra.h"
#include "timer.h"

// Defines for GPIO memory location and CFG0 offset from that location
#define GPIO        0x02000000
#define PB_CFG0_OFF 0x30

// You call assert on an expression that you expect to be true. If expr
// instead evaluates to false, then assert calls abort, which stops
// your program and flashes onboard led.
#define assert(expr) if(!(expr)) abort()

// infinite loop that flashes onboard blue LED (GPIO PD18)
static void abort(void) {
    volatile unsigned int *GPIO_CFG2 = (unsigned int *)0x02000098;
    volatile unsigned int *GPIO_DATA = (unsigned int *)0x020000a0;

    // Configure GPIO PD18 function to be output.
    *GPIO_CFG2 = (*GPIO_CFG2 & ~(0xf00)) | 0x100;
    while (1) { // infinite loop
        *GPIO_DATA ^= (1 << 18); // invert value
        for (volatile int delay = 0x100000; delay > 0; delay--) ; // wait
    }
}

// Enumeration (in bit encoding) of PULL register two-bit (pun unintentional) states:

enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP   = 1,
    GPIO_PULL_DOWN = 2,
};

// Helper functions and structs copied over from gpio.c, since they are not defined in gpio.h
typedef struct {
    unsigned int group;
    unsigned int pin_index;
} gpio_pin_t;

// The gpio_id_t enumeration assigns a symbolic constant for each
// in such a way to use a single hex constant. The more significant
// hex digit identifies the group and lower 2 hex digits are pin index:
//       constant 0xNnn  N = which group,  nn = pin index within group
//
// This helper function extracts the group and pin index from a gpio_id_t
// e.g. GPIO_PB4 belongs to GROUP_B and has pin_index 4
static gpio_pin_t get_group_and_index(gpio_id_t gpio) {
    gpio_pin_t gp;
    gp.group = gpio >> 8;
    gp.pin_index = gpio & 0xff; // lower 2 hex digits
    return gp;
}

// Helper function that returns whether a GPIO input function is valid
// (not calling a reserved function and within 4 bits, that is, < 16).
static bool gpio_func_is_valid(unsigned int function) {
    return (function < 16) && (function < 9 || function > 13);
}

// Helper function suggestion that returns address of config0
// register for a gpio group, i.e. get_cfg0_reg(GROUP_B)
// Refer to D1-H user manual to identify address of config0 register
// for each group. It can be helpful to note how address of config1 and
// config2 registers can be computed as relative offset from config0.
// (can discard this function if it doesn't fit with your design)
static volatile unsigned int *get_cfg0_reg(unsigned int group) {
    // Aborts with invalid request return if an invalid gpio group number (> 5) is specified.
    if(group > 5) { return (volatile unsigned int*)GPIO_INVALID_REQUEST; }
    
    // CFG0 registers for each gpio group are separated by 0x30 bytes in sucession.
    return (volatile unsigned int*)GPIO + ((PB_CFG0_OFF + (0x30*group))/4);
}

// Helper function to get address of PULL0 register for each gpio group.
static volatile unsigned int *get_pull0_reg(unsigned int group) {
    return get_cfg0_reg(group)+(0x24/4); // PULL0 registers for gpio group always 36 bytes (9 ints) after CFG0 reg.
}

// Helper function to set a generalized two-bit pull state in the correct gpio group PULL register at the correct location
void gpio_set_pull_state(gpio_id_t pin, unsigned int pull_state) {
    // Aborts function call if gpio pin is invalid
    if(!gpio_id_is_valid(pin)) { return; }
    
    // Aborts function call if gpio pull state is invalid (is more than 2 bits or is the reserved state of 3)
    if(((pull_state & 0xfffffffc) != 0) || pull_state == 3) { return; }

    // Calculates pointer to memory of the desired gpio pin in the pull0 register
    gpio_pin_t pin_loc = get_group_and_index(pin);
    volatile unsigned int* pull_ptr = get_pull0_reg(pin_loc.group)+(pin_loc.pin_index/16); // Uses rounding rules of integer division in C to our advantage!
   
    // The gpio pull state enums are fortunately already in the correct bitmasking once reduced to their 2 least significant bits!
    // The trick is to correctly mask them over the preexisting register bits at the right (valid) data location! This is what I do:

    // Wipes previous configuration data at desired 2-bit interval of PULL register.
    unsigned int two_bits_of_annihilation = 0x3;
    two_bits_of_annihilation <<= (pin_loc.pin_index*2);
    two_bits_of_annihilation ^= 0xffffffff;
    *pull_ptr &= two_bits_of_annihilation;

    // Sets desired 2-bit interval to our desired pull state
    pull_state <<= (pin_loc.pin_index*2);
    *pull_ptr |= pull_state;
}

// Helper function to get the two-bit pull state of any gpio pin :)
unsigned int gpio_get_pull_state(gpio_id_t pin) {
    // Aborts function with return of invalid request if the pin provided is invalid
    if(!gpio_id_is_valid(pin)) { return GPIO_INVALID_REQUEST; }

    // Calculates pointer to memory of the desired gpio pin in the pull0 register
    gpio_pin_t pin_loc = get_group_and_index(pin);
    volatile unsigned int* pull_ptr = get_pull0_reg(pin_loc.group)+(pin_loc.pin_index/16); // Uses rounding rules of integer division in C to our advantage!
    
    unsigned int pull_state = *pull_ptr; // Derefences 4 bytes after the pull start pointer to load into function.
    // Reduces 32-bit pull block to only the desired GPIO pins' function, then returns.
    pull_state >>= (pin_loc.pin_index*2);
    pull_state &= 0x00000003;
    return pull_state;
}

void gpio_set_pullup(gpio_id_t pin) {
    gpio_set_pull_state(pin, GPIO_PULL_UP);
}

void gpio_set_pulldown(gpio_id_t pin) {
    gpio_set_pull_state(pin, GPIO_PULL_DOWN);
}

void gpio_set_pullnone(gpio_id_t pin) {
    gpio_set_pull_state(pin, GPIO_PULL_NONE);
}
/*
void main(void) {
    // Basic testing of pull state adjustments to some gpio pins:
    gpio_set_pullup(GPIO_PB0);
    assert(gpio_get_pull_state(GPIO_PB0) == GPIO_PULL_UP);

    gpio_set_pulldown(GPIO_PB1);
    assert(gpio_get_pull_state(GPIO_PB1) == GPIO_PULL_DOWN);

    gpio_set_pullnone(GPIO_PB0);
    assert(gpio_get_pull_state(GPIO_PB0) == GPIO_PULL_NONE);

    gpio_set_pullnone(GPIO_PB1);
    assert(gpio_get_pull_state(GPIO_PB1) == GPIO_PULL_NONE);
}
*/
