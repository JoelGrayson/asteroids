#pragma once

#include "gl.h"
#include "constants.h"
#include <stdbool.h>
#include "printf.h"

// Whether you are waiting on the start game screen. Set to false by a button click. start_game_screen does not move on until this is false
extern bool waiting;

/** Asks user to press any button before continuing to start the game */
void start_game_screen();

