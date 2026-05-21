/* File: gpio.c
 * ------------
 * ***** D1-H Mango Pi microcontroller GPIO pin basic control and monitoring functions implemented here. *****
 */
#include <stdbool.h>
#include "gpio.h"

// Defines for GPIO memory location and CFG0 offset from that location
#define GPIO         0x02000000
#define PB_CFG0_OFF  0x30

enum { GROUP_B = 0, GROUP_C, GROUP_D, GROUP_E, GROUP_F, GROUP_G };
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

// The gpio groups are differently sized, e.g. B has 13 pins, C only 8.
// This helper function confirms that a gpio_id_t is valid (group
// and pin index are valid)
bool gpio_id_is_valid(gpio_id_t pin) {
    gpio_pin_t gp = get_group_and_index(pin);
    switch (gp.group) {
        case GROUP_B: return (gp.pin_index <= GPIO_PB_LAST_INDEX);
        case GROUP_C: return (gp.pin_index <= GPIO_PC_LAST_INDEX);
        case GROUP_D: return (gp.pin_index <= GPIO_PD_LAST_INDEX);
        case GROUP_E: return (gp.pin_index <= GPIO_PE_LAST_INDEX);
        case GROUP_F: return (gp.pin_index <= GPIO_PF_LAST_INDEX);
        case GROUP_G: return (gp.pin_index <= GPIO_PG_LAST_INDEX);
        default:      return false;
    }
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

    // CFG0 registers for each gpio group are separated by 0x30 bytes in succession.
    //volatile unsigned int* cfg0_reg = (volatile unsigned int*)GPIO + cfg0_reg += (PB_CFG0_OFF + (0x30*group));
    return (volatile unsigned int*)GPIO + ((PB_CFG0_OFF + (0x30*group))/4);
}

// Similar to above, get address of data register for a gpio group
// (can discard this function if it doesn't fit with your design)
static volatile unsigned int *get_data_reg(unsigned int group) {
    return get_cfg0_reg(group)+(0x10/4); // DAT registers for gpio group always 16 bytes (4 ints) after CFG0 register. So can re-use code/error checking from b4.
}

void gpio_init(void) {
    // no initialization required for this peripheral
}

void gpio_set_input(gpio_id_t pin) {
    gpio_set_function(pin, GPIO_FN_INPUT);
}

void gpio_set_output(gpio_id_t pin) {
    gpio_set_function(pin, GPIO_FN_OUTPUT);
}

void gpio_set_function(gpio_id_t pin, unsigned int function) {
    // Aborts function call if gpio pin is invalid
    if(!gpio_id_is_valid(pin)) { return; }
    
    // Aborts function call if gpio function is invalid
    if(!gpio_func_is_valid(function)) { return; }

    // Calculates pointer to memory of the desired gpio pin configuration register
    gpio_pin_t pin_loc = get_group_and_index(pin);
    volatile unsigned int* cfg_ptr = get_cfg0_reg(pin_loc.group)+(pin_loc.pin_index/8); // Uses rounding rules of integer division in C to our advantage!
   
    // The gpio function enums are fortunately already in the correct bitmasking once reduced to their 4 least significant bits!
    // The trick is to correctly mask them over the preexisting register bits at the right (valid) data location! This is what I do:

    // Wipes previous configuration data at desired 4-bit interval of CFG register.
    unsigned int four_bits_of_annihilation = 0xf;
    four_bits_of_annihilation <<= (pin_loc.pin_index*4);
    four_bits_of_annihilation ^= 0xffffffff;
    *cfg_ptr &= four_bits_of_annihilation;

    // Sets desired 4-bit interval to our desired function
    function <<= (pin_loc.pin_index*4);
    *cfg_ptr |= function;
}

unsigned int gpio_get_function(gpio_id_t pin) {
    // Aborts function with return of invalid request if the pin provided is invalid
    if(!gpio_id_is_valid(pin)) { return GPIO_INVALID_REQUEST; }

    // Calculates pointer to memory of the desired gpio pin configuration register
    gpio_pin_t pin_loc = get_group_and_index(pin);
    volatile unsigned int* cfg_ptr = get_cfg0_reg(pin_loc.group)+(pin_loc.pin_index/8); // Uses rounding rules of integer division in C to our advantage!
    
    unsigned int function = *cfg_ptr; // Derefences 4 bytes after the cfg start pointer to load into function.
    // Reduces 32-bit config block to only the desired GPIO pins' function, then returns.
    function >>= (pin_loc.pin_index*4);
    function &= 0x0000000f;
    return function;
}

void gpio_write(gpio_id_t pin, int value) {
    // Aborts function if gpio pin provided is invalid
    if(!gpio_id_is_valid(pin)) { return; }
    // Aborts function if gpio pin is not already set to OUTPUT
    if(gpio_get_function(pin) != GPIO_FN_OUTPUT) { return; }
    
    // Aborts for invalid non-single-bit value inputs to gpio_write.
    if((value & 0xfffffffe) != 0) { return; }

    // Calculates pointer to memory of the desired gpio pin data register
    gpio_pin_t pin_loc = get_group_and_index(pin);
    volatile unsigned int* dat_ptr = get_data_reg(pin_loc.group);
   
    // Wipes previous data at desired DAT register bit.
    unsigned int bit_of_annihilation = 1;
    bit_of_annihilation <<= (pin_loc.pin_index);
    bit_of_annihilation ^= 0xffffffff;
    *dat_ptr &= bit_of_annihilation;

    // Sets desired data register bit to our desired value
    value <<= (pin_loc.pin_index);
    *dat_ptr |= value;
}

int gpio_read(gpio_id_t pin) {
    // Checks if gpio pin type provided is an actually valid pin
    if(!gpio_id_is_valid(pin)) { return GPIO_INVALID_REQUEST; }
    
    // Calculates pointer to memory of the desired gpio pin data register
    gpio_pin_t pin_loc = get_group_and_index(pin);
    volatile unsigned int* dat_ptr = get_data_reg(pin_loc.group);
    
    // Reads desired DAT register bit into integer.
    int bit_read = 1;
    bit_read <<= (pin_loc.pin_index);
    bit_read &= *dat_ptr;
    bit_read >>= (pin_loc.pin_index);
    return bit_read;
}
