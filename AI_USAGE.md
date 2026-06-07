# AI Usage
* Rotation matrix equation
* Center-align my rocket and explosion points
* Learned that NULL can be assigned to any pointer type and `= { 0 }` is how you set a new array to all NULL (instead of garbage value because not zeroed out by default). Also if (x == NULL) can be written as if (!x) because of the falsiness of NULL
* Used Claude cowork to convert the .wav audio files from 8 bit to 16 bit and 11 kHz to 48 kHz
* Learned that you can't do const int ROCKET_NUM_POINTS = 6 and then create a constant array length ROCKET_NUM_POINTS because even though const int it still treats it like an int. Compiler is confused and things it can be variable-length and cosntant at same time so throws an error. Soln is to use a #define.
* Centered asteroid coordinates using AI.
* Realized there needed to be an include graphics/point.h in order to get the .c to work with point
* using find . -name '*.o' -delete instead of rm because `rm **/*.o` ignores current directory in bash/sh even though this is fine in zsh
* debugging
  * turns out I had the same guard header ASTEROID_H in draw_asteroid.h and asteroid.h which led to issues
  * turns out I was missing #include "point.h" from my asteroid.h
* how to use extern with asteroid points definition. The answer is extern makes a declaratoin whihc you need in a .h file because if you declared it in .h then the multiple files that include it would lead to multiple redeclarations of the same file but marking as extern si like forward declaration and the .c can define it
* Makefile
  * catching that I forgot to include .o for some stuff like button.o in OTHER_SOURCES
  * multiple gpio_extras including issues: added to ld flags: --allow-multiple-definition
  * clang: error: unsupported argument 'rv64im_zicsr' to option '-march='
    * Turns out my target was wrong
* SD Card
  * Used Stanford AI Playground's copy of Claude 4-5 Sonnet to translate the "write mmc image to bootloader" script in the CS107E SD card boot loader library
  * from MacOS shell script to Windows 11 Powershell;
    * that somehow worked.


