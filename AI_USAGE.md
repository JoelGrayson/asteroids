# Figuring out
* how to use extern with asteroid points definition. The answer is extern makes a declaratoin whihc you need in a .h file because if you declared it in .h then the multiple files that include it would lead to multiple redeclarations of the same file but marking as extern si like forward declaration and the .c can define it
* Makefile
  * clang: error: unsupported argument 'rv64im_zicsr' to option '-march='
    * Turns out my target was wrong


