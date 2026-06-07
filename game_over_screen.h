#pragma once

#include "gl.h"
#include <stdbool.h>
#include "constants.h"

extern int high_scores[10];
extern char* high_score_names[10];

// Sets up/prepares gameover screen:
void setup_gameover();

// Returns whether is gameover or not.
bool is_gameover();

/** Asks user to press any button before continuing to start the game */
void game_over_screen();
void restart_game(); // Function to be called when full name is entered for high score!

// Letter start rotation/end rotation for end screen, going up (Z->A).
void letter_set_rotating_up();
void letter_set_unrotating_up();

// Letter start rotation/end rotation for end screen, going down (A->Z).
void letter_set_rotating_down();
void letter_set_unrotating_down();

// Letter entering function in end screen
void letter_enter();

// Sets all initial high scores to placeholder at initial boot of game.
void reset_highscores();