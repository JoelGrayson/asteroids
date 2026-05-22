#include "maths.h"
#include "malloc.h"

// Trigonometric data to load upon trig math intialization
static double* sine_table;
static unsigned int trig_precision;
static double trig_precision_scale;

// Handy number theory functions
double pow(double base, unsigned int exponent)
{
    double ret = 1;
    for(int i = 0; i < exponent; i++) {
        ret *= base;
    }
    return ret;
}

unsigned int factorial(unsigned int power) {
    int ret = 1;
    while(power != 0) {
        ret = ret * power;
        power--;
    }
    return ret;
}

void trig_init(unsigned int precision) {
    // Determines trig calculations precision scaling
    trig_precision = precision;
    trig_precision_scale = pow(10, precision);
    
    // Determines size for sine calculations table and allocates it in memory
    int table_len = (int)(2*PI*trig_precision_scale);
    sine_table = malloc(sizeof(double)*table_len);

    double angle = 0;
    double angle_it = 1/trig_precision_scale;
    for(int i = 0; i < table_len; i++) { // Raw taylor series expansion calculations of sine function
        sine_table[i] = angle - (pow(angle,3)/(double)factorial(3)) + (pow(angle,5)/(double)factorial(5));
        for(int j = 0; j < precision-2; j++) {
            sine_table[i] = angle - (pow(angle,5+(2*j))*pow(-1, (j & 1))/(double)factorial(5+2*j));
        }
        angle += angle_it; // Going through all angles which sine could be called for.
    }
}

// Basic trig functions
double sine(double angle) {
    angle = angle % (2*PI); // Reduces angle to its simplest equivalent.
    int index = (int)(angle*trig_precision_scale); // Calculates index that the sin of said angle SHOULD BE AT, or is closest to it.
    return sine_table[index];
}
double cosine(double angle) {
    return sine(angle+(PI/2));
}
double tan(double angle) {
    return sine(angle)/cos(angle);
}
