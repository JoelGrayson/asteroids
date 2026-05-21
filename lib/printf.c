/* File: printf.c
 * --------------
 * Functions for formatting strings using % formatters and printing to the UART console
 */
#include "printf.h"
#include <stdarg.h>
#include <stdint.h>
#include "strings.h"
#include "uart.h"

/* Prototypes for internal helpers.
 * Typically we would qualify these functions as static (private to module)
 * but in order to call them from the test program, must declare externally
 */
void num_to_string(unsigned long num, int base, char *outstr);
const char *hex_string(unsigned long val);
const char *decimal_string(long val);

// max number of digits in long + space for negative sign and null-terminator
#define MAX_DIGITS 25


const char *SPACE = " "; //used for padding
const char *ZERO = "0"; //used for padding


/* Convenience functions `hex_string` and `decimal_string` are provided
 * to you.  You should use the functions as-is, do not change the code!
 *
 * A key implementation detail to note is these functions declare
 * a buffer to hold the output string and return the address of buffer
 * to the caller. If that buffer memory were located on stack, it would be
 * incorrect to use pointer after function exit because local variables
 * are deallocated. To ensure the buffer memory is accessible after
 * the function exists, the declaration is qualified `static`. Memory
 * for a static variable is not stored on stack, but instead in the global data
 * section, which exists outside of any function call. Additionally static
 * makes it so there is a single copy of the variable, which is shared by all
 * calls to the function. Each time you call the function, it overwrites/reuses
 * the same variable/memory.
 *
 * Adding static qualifier to a variable declared inside a function is a
 * highly atypical practice and appropriate only in very specific situations.
 * You will likely never need to do this yourself.
 * Come talk to us if you want to know more!
 */

const char *hex_string(unsigned long val) {
    // static buffer to allow use after function returns (see note above)
    static char buf[MAX_DIGITS];
    num_to_string(val, 16, buf); // num_to_string does the hard work
    return buf;
}

const char *decimal_string(long val) {
    // static buffer to allow use after function returns (see note above)
    static char buf[MAX_DIGITS];
    if (val < 0) {
        buf[0] = '-';   // add negative sign in front first
        num_to_string(-val, 10, buf + 1); // pass positive val as arg, start writing at buf + 1
    } else {
        num_to_string(val, 10, buf);
    }
    return buf;
}

static char digit_to_char(int digit) {
    if (digit >= 0 && digit <= 9) {
        return digit + '0';
    }
    if (digit >= 10 && digit <= 15) { //a–e. Map 10 to 'a'
        return digit - 10 + 'a';
    }
    return '!'; //return ! to indicate error
}

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

int vsnprintf(char *buf, size_t bufsize, const char *format, va_list args) {
    // snprintf has only one forward pass over format (so va_list works and only work with one buffer)
    // It reads format character-by-character, at each character, figuring out what string to add to buf (next_string_to_add)
    // If it read a normal character from format, next_string_to_add is just that character + '\0'
    // If it read a %, go to the next character and then figure out what string needs to be added, create it in constructed_string and make next_string_to_add point to it
    // Then use strlcat to add next_string_to_add with bufsize - 1 as the limit, so that it will never exceed the max buf_size
    // strlcat will return how much space it would have taken to make the full string, so you can count them all up in out
    /* Pseudocode:
    out = 0 //total length of the string were there no bufsize cap
    while format not exhausted
        char constructed_string[32]
        char *next_string_to_add = constructed_string //point to the newly-created string
        if *format == %
            if format[1] is a number, use strtonum to extract field width and have endptr update format
            switch *(format+1)
                case %: next_string_to_add = "%"
                case s: next_string_to_add = va_arg char* //point to the provided string
                case d: 
                    num_to_string(va_arg int, save_to=constructed_string)
                case c:
                    constructed_string[0] = va_arg char
                    constructed_string[1] = '\0'
                case x:
                    constructed_string[0] = '0'
                    constructed_string[1] = 'x'
                    num_to_string(va_arg int, 16, save_to = constructed_string + 2)
                case p: next_string_to_add = 
                default: constructed_string = <UNKNOWN FORMAT %a>
            
            format += 2 //we just read two characters off of format
        else // Just a plain ol' char
            constructed_string[0] = *format
            constructed_string[1] = '\0'

            format++ //we just read only one character off of format
        
        out += strlcat(next_string_to_add, bufsize) //if remaining space is 0, this does nothing
    return out
    */

    if (bufsize > 0) {
        // Ensures strlcat starts adding from the start
        // Otherwise, if the first character is not \0, strlcat will start adding down the line
        *buf = '\0';
    }

    int out = 0; //total length of the string were there no bufsize cap

    while (*format != '\0') { //while format not exhausted
        // This needs to be at the same scope level as next_string_to_add and the strlcat below
        // so next_string_to_add is not pointing to a dangling pointer
        const int CONSTRUCTED_STRING_LENGTH = 32;
        char constructed_string[CONSTRUCTED_STRING_LENGTH]; //declare new string on stack

        char *next_string_to_add = constructed_string;

        int field_width = -1; //-1 means no field_width constraint
        const char *separator = SPACE;

        if (*format == '%') { //special character sequence
            format++; // *format now reads either a number or the symbol

            // If after % is a number, read it into field_width and advance format forward so *forward is the symbol
            if (*format >= '0' && *format <= '9') {
                field_width = strtonum(
                    format,
                    &format //chop of the number
                );
            }
            
            switch (*format) { //determines the type of special sequence
                case '%': {
                    constructed_string[0] = '%';
                    constructed_string[1] = '\0';
                    break;
                }
                case 's': {
                    next_string_to_add = va_arg(args, char *); //string to insert, client provided pointer
                    break;
                }
                case 'c': {
                    constructed_string[0] = (char) va_arg(args, int); //compiler said to use int not char as the va_arg type
                    constructed_string[1] = '\0';
                    break;
                }
                case 'd': {
                    int arg = va_arg(args, int);
                    //num_to_string(arg, 10, constructed_string); //taken out because doesn't work with negative numbers
                    const char *returned_string = decimal_string(arg);
                    memcpy(constructed_string, returned_string, strlen(returned_string) + 1);
                    break;
                }
                case 'x': {
                    int arg = va_arg(args, int);
                    const char *returned_string = hex_string(arg);
                    memcpy(constructed_string, returned_string, strlen(returned_string) + 1);
                    separator = ZERO;
                    break;
                }
                case 'l': { //l works with %ld and %lx
                    long arg = va_arg(args, long);
                    format++; //now *format is d, x, or something else
                    if (*format == 'd') { //ld
                        const char *returned_string = decimal_string(arg);
                        memcpy(constructed_string, returned_string, strlen(returned_string) + 1);
                    } else if (*format == 'x') { //lx
                        const char *returned_string = hex_string(arg);
                        memcpy(constructed_string, returned_string, strlen(returned_string) + 1);
                        separator = ZERO;
                    } else { // <UNKNOWN FORMAT %lz>
                        memcpy(constructed_string, "<UNKNOWN FORMAT %l_>", 21);
                        constructed_string[18] = *format; //replace _ in %_ with the actual format letter
                    }
                    break;
                }
                case 'p': {
                    // Pointer starts with 0x
                    strlcat(buf, "0", bufsize);
                    strlcat(buf, "x", bufsize);
                    out += 2; //account for "0x" in full width string

                    if (field_width == -1) { //default field_width for pointer is 8, however can be overwritten
                        field_width = 8;
                    }
                    
                    long arg = va_arg(args, long);
                    const char *returned_string = hex_string(arg);
                    memcpy(constructed_string, returned_string, strlen(returned_string) + 1);
                    separator = ZERO;
                    break;
                }
                case 'm': {
                    constructed_string[0] = '$';
                    double money = va_arg(args, double);
                    // The $ amount
                    int dollars = (int) money;
                    const char *number_as_string = decimal_string(dollars);
                    int length = strlen(number_as_string);

                    for (int i = 0; i < length; i++) {
                        char new_digit_to_add[3] = {'\0'}; //may have a comma after it or not
                        int place = length - 1 - i; //tens would have place = 1, ten-thousands has place = 4
                        new_digit_to_add[0] = number_as_string[i];
                        if (place % 3 == 0 && place != 0) { //every third place, with the exception of the ones place has a comma after it
                            new_digit_to_add[1] = ',';
                            new_digit_to_add[2] = '\0';
                        } else {
                            new_digit_to_add[1] = '\0';
                        }
                        strlcat(constructed_string, new_digit_to_add, CONSTRUCTED_STRING_LENGTH);
                    }

                    if (dollars == 0) { //so .21 is formatted as $0.21
                        strlcat(constructed_string, "0", CONSTRUCTED_STRING_LENGTH);
                    }

                    // Add cents
                    char cents[] = ".xx";
                    cents[1] = '0' + ((int)(money * 10) % 10); //dimes
                    cents[2] = '0' + ((int)(money * 100) % 10); //pennies

                    strlcat(constructed_string, cents, CONSTRUCTED_STRING_LENGTH);


                    // strlcat(constructed_string, );
                    break;
                }
                default: { //if invalid sequence like %a, print "<UNKNOWN FORMAT %a>"
                    memcpy(constructed_string, "<UNKNOWN FORMAT %_>", 20);
                    constructed_string[17] = *format; //replace _ in %_ with the actual format letter
                    break;
                }
            }


            format++; //chop off the symbol
        } else {
            constructed_string[0] = *format;
            constructed_string[1] = '\0';

            format++; //read one character off of format
        }

        size_t next_string_to_add_length = strlen(next_string_to_add); //add length contribution to full size string
        
        // Use field width to pad with spaces if applicable
        if (
            field_width != -1 //if field_width constraint
            &&
            field_width > next_string_to_add_length //if there needs to be padding added
        ) {
            int amount_to_pad = field_width - next_string_to_add_length;
            for (int i = 0; i < amount_to_pad; i++) {
                strlcat(buf, separator, bufsize); //add spaces so it grows to field_width
                out++;
            }
        }
        
        // Using strlcat means the bufsize will not be exceeded when writing to buf
        strlcat(buf, next_string_to_add, bufsize);

        out += next_string_to_add_length;
    }
    
    return out;
}

int snprintf(char *buf, size_t bufsize, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int out = vsnprintf(buf, bufsize, format, args);

    va_end(args);

    return out;
}

int printf(const char *format, ...) {
    const int MAX_OUTPUT_LEN = 1024; // ok to assume printf output never longer than this

    char buf[MAX_OUTPUT_LEN];

    va_list args;
    va_start(args, format);

    int out = vsnprintf(buf, MAX_OUTPUT_LEN, format, args);
    
    va_end(args);

    uart_putstring(buf);

    return out;
}
