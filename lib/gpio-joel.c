/* File: gpio.c
 * ------------
 * Control the GPIO with these functions
 */
#include "gpio.h"
#include <stddef.h>

enum { GROUP_B = 0, GROUP_C, GROUP_D, GROUP_E, GROUP_F, GROUP_G };

typedef struct  {
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

// Helper function suggestion that returns address of config0
// register for a gpio group, i.e. get_cfg0_reg(GROUP_B)
// Refer to D1-H user manual to identify address of config0 register
// for each group. It can be helpful to note how address of config1 and
// config2 registers can be computed as relative offset from config0.
// (can discard this function if it doesn't fit with your design)
static volatile unsigned int *get_cfg0_reg(unsigned int group) {
    // 0x20...30 is PB_CFG0
    // 0x20...60 is PC_CFG0 //this is 0x30 ahead and each int represents 0x04 so this is 12 required. 0x30 / 4 = 48 / 4 = 12
    // 0x20...90 is PD_CFG0
    
    long addr = 0x02000000 + 0x30;
    addr += 0x30 * group;
    return (volatile unsigned int *)addr;
}

// Similar to above, get address of data register for a gpio group
// (can discard this function if it doesn't fit with your design)
static volatile unsigned int *get_data_reg(unsigned int group) {
    // 0x20...40 is PB_DATA
    // 0x20...70 is PC_DATA

    long addr = 0x02000000 + 0x40;
    addr += 0x30 * group;
    return (volatile unsigned int *)addr;
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

/** gpio_set_function takes in a pin and sets it to a function
 * It must take the address in cfg_0_reg and update it to a new value at the pin index to function
 */
void gpio_set_function(gpio_id_t pin, unsigned int function) {
    bool invalid_pin = !gpio_id_is_valid(pin);
    bool invalid_function = function > 0b1111;
    if (invalid_pin || invalid_function) //don't do anything if invalid
        return;


        
    gpio_pin_t gp = get_group_and_index(pin);

    volatile unsigned int *cfg_0_reg = get_cfg0_reg(gp.group);
    unsigned int pin_index = gp.pin_index;
    // Each memory address points to an unsigned int, of 32 bits. That is 8 x 4, which is for 8 GPIO pins functions. Thus, after 8 GPIO pins, you should point to the next word.
    if (pin_index < 8) { // 0 to 7
        // CFG0
    } else if (pin_index >= 8 && pin_index <= 15) {
        // CFG1. Go from 0x30 to 0x34 offset. That is, the address goes up by 4 bytes or 8 nibbles.
        cfg_0_reg++;
    } else if (pin_index >= 16) {
        // CFG2 is 8 bits above cfg_0
        cfg_0_reg += 2;
    }
    pin_index = pin_index % 8;

    volatile unsigned int old_value = *cfg_0_reg; //read old value

    volatile unsigned int mask = ~(0b1111 << pin_index * 4); // 1111 1111 1111 1111 1111 0000 1111 1111 for pin_index==2
    volatile unsigned int erased_the_correct_nibble = old_value & mask;
    volatile unsigned int inserted_the_correct_nibble = erased_the_correct_nibble | (function << (pin_index * 4));
    *cfg_0_reg = inserted_the_correct_nibble; //write new value
}

unsigned int gpio_get_function(gpio_id_t pin) {
    bool is_valid = gpio_id_is_valid(pin);
    if (!is_valid)
        return GPIO_INVALID_REQUEST;

    gpio_pin_t gp = get_group_and_index(pin);

    volatile unsigned int *cfg_0_reg = get_cfg0_reg(gp.group);
    unsigned int pin_index = gp.pin_index;
    // Each memory address points to an unsigned int, of 32 bits. That is 8 x 4, which is for 8 GPIO pins functions. Thus, after 8 GPIO pins, you should point to the next word.
    if (pin_index < 8) { // 0 to 7
        // CFG0
    } else if (pin_index >= 8 && pin_index <= 15) {
        // CFG1. Go from 0x30 to 0x34 offset. That is, the address goes up by 4 bytes or 8 nibbles.
        cfg_0_reg++;
    } else if (pin_index >= 16) {
        // CFG2 is 8 bits above cfg_0
        cfg_0_reg += 2;
    }
    pin_index = pin_index % 8;

    volatile unsigned int whole_word = *cfg_0_reg; //read the word. 8 nibbles of data. Need to bitshift to find the right one
    volatile unsigned int the_value = (whole_word >> (pin_index * 4)) & 0b1111; //just get the correct nibble
    return the_value;
}

void gpio_write(gpio_id_t pin, int value) {
    bool is_valid = gpio_id_is_valid(pin);
    if (!is_valid) //don't do anything if invalid
        return;

    gpio_pin_t gp = get_group_and_index(pin);
    volatile unsigned int *addr = get_data_reg(gp.group);

    // Returns a word of data. First 12 (PB) or 23 (PD) bits is the data. Rest is irrelevant
    volatile unsigned int old_value = *addr;
    volatile unsigned int mask = ~(1 << gp.pin_index); //11111011
    volatile unsigned int erased = old_value & mask;
    volatile unsigned int new_value = erased | (value << gp.pin_index);

    *addr = new_value;
}

int gpio_read(gpio_id_t pin) {
    bool is_valid = gpio_id_is_valid(pin);
    if (!is_valid)
        return GPIO_INVALID_REQUEST;

    gpio_pin_t gp = get_group_and_index(pin);
    volatile unsigned int *addr = get_data_reg(gp.group);

    // Returns a word of data. First 12 bits is the data. Rest is irrelevant
    volatile unsigned int whole_word = *addr;
    volatile int the_value = (whole_word >> gp.pin_index) & 0b1;
    return the_value;
}

