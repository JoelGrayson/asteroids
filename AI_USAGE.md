# Figuring out
* Centered asteroid coordinates using AI.
* Realized there needed to be an include graphics/point.h in order to get the .c to work with point
* using find . -name '*.o' -delete instead of rm because `rm **/*.o` ignores current directory in bash/sh even though this is fine in zsh
* debugging
  * turns out I had the same guard header ASTEROID_H in draw_asteroid.h and asteroid.h which led to issues
  * turns out I was missing #include "point.h" from my asteroid.h
* how to use extern with asteroid points definition. The answer is extern makes a declaratoin whihc you need in a .h file because if you declared it in .h then the multiple files that include it would lead to multiple redeclarations of the same file but marking as extern si like forward declaration and the .c can define it
* Makefile
  * multiple gpio_extras including issues: added to ld flags: --allow-multiple-definition
  * clang: error: unsupported argument 'rv64im_zicsr' to option '-march='
    * Turns out my target was wrong


