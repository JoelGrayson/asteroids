#include "num_to_string.h"

void num_to_string(unsigned long num, int base, char *outstr) {
    /*  Pseudocode:
        1. Instantiate a character buffer, digits
        2. Write num to digits
            Take the LSB of num and stuff it into digits
            Divide num by 10 to shift it over
            Repeat until num is 0
        3. Write digits to outstr
            Starting at the end of digits, write in reverse order to outstr
            
    */

    // 1.
    // long cannot be more than 64 bits wide, which is 64/3 (8) = 21.3
    // Thus, 24 characters should be more than enough to represent in base 10,
    // and of course in base 16 too.
    char digits[24]; // digits[0] is the ones place of the resulting outstr, digits[1] the tens place, ...
    char *cursor = digits; //in part 2, always points to one past the digits characters that were inserted into digits_i

    // 2.
    while (num > 0) {
        // Extract LSB
        int digit = num % base;
        num /= base;
        
        *cursor++ = digit_to_char(digit);
    }

    // 3.
    while (cursor != digits) { //until you've popped out all the digits
        *outstr++ = *--cursor; //what a beauty
    }
    *outstr = '\0'; //null-terminator
}

