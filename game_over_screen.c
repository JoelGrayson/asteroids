#include "start_game_screen.h"
#include "game_manager_state.h"
#include "strings.h"
#include "timer.h"
#include "interrupts.h"
#include "score_and_lives.h"
#include <stdbool.h>

static char name[4];
static bool gameover = false;

// Letter selection variables
static char letter_selected = 'A'-1;
static bool rotating_down = false;
static bool rotating_up = false;
static int let_index = 0;
static int highscore_index = -1;
static unsigned long last_rotate_tick;

// BEGIN PERSIST
int high_scores[10];
char* high_score_names[10];
// END PERSIST

static char empty[1];
static bool score_loaded = false;

#define ROTATE_PERIOD 200000*TICKS_PER_USEC

// Resets start and end screen states for new game start!
void restart_game() {
    waiting = true;
    gameover = false;
    score_loaded = false;
    game_manager_state = START_GAME_SCREEN;
}

void reset_highscores() {
    empty[0] = '\0';
    // Sets all highscores to placeholder -1.
    memset(&high_scores[0], -1, 10*sizeof(int));
    // Sets all names to point to an empty string at start (placeholder).
    for(int i = 0; i < 10; i++) {
        high_score_names[i] = &empty[0];
    }
    highscore_index = -1;
}

static void load_highscore() {
    if(!score_loaded) {
        // Updates highscore list, if possible.
        int score = get_score(); // Gets score of game played!
        int old_score;
        bool new_highscore = false;
        for(int i = 0; i < 10; i++) {
            old_score = high_scores[i];
            if(score > old_score) {
                new_highscore = true;
                highscore_index = i; // Index at which to place new highscore name!

                int tail = 9;
                while(tail > i) {
                    high_scores[tail] = high_scores[tail-1];
                    high_score_names[tail] = high_score_names[tail-1];
                    tail--;
                }
                // Places new highscore.
                high_scores[i] = score;
                score_loaded = true;
                return;
            }
        }
        // If no new highscore found, immediately restart game, don't show endscreen.
        if(!new_highscore) {
            highscore_index = -1; // No load highscore name!
            restart_game();
        }
    }
}

static void rotate_letter_down() {
    letter_selected = (letter_selected+1 > 'Z') ? 'A' : letter_selected+1;
}

static void rotate_letter_up() {
    letter_selected = (letter_selected-1 < 'A') ? 'Z' : letter_selected-1;
}

static void rotate_letter() {
    if(timer_get_ticks() - last_rotate_tick > ROTATE_PERIOD) {
        if(rotating_down) {
            rotate_letter_up();
            name[let_index] = letter_selected;
            last_rotate_tick = timer_get_ticks();
        } else if(rotating_up) {
            rotate_letter_down();
            name[let_index] = letter_selected;
            last_rotate_tick = timer_get_ticks();
        }
    }
}

void setup_gameover() {
    memset(name, 0, 4);
    name[0] = '_';
    name[1] = '_';
    name[2] = '_';
    gameover = true;
    let_index = 0;
}

bool is_gameover() {
    return gameover;
}

void game_over_screen() {
    const double char_width = gl_get_char_width();
    
    load_highscore(); // Loads highscore if we made it!
    rotate_letter();
    gl_clear(GL_BLACK);

    gl_draw_string(MONITOR_WIDTH / 2 - 300, MONITOR_HEIGHT * 0.2, "Your score is one of the ten best", GL_WHITE);
    gl_draw_string(MONITOR_WIDTH / 2 - 300, MONITOR_HEIGHT * 0.25, "Please enter your initials", GL_WHITE);
    gl_draw_string(MONITOR_WIDTH / 2 - 300, MONITOR_HEIGHT * 0.30, "Push ROTATE to select letter", GL_WHITE);
    gl_draw_string(MONITOR_WIDTH / 2 - 300, MONITOR_HEIGHT * 0.35, "Push HYPERSPACE when letter is correct", GL_WHITE);
    
    gl_draw_char(MONITOR_WIDTH / 2 - 200, MONITOR_HEIGHT * 0.47, name[0], GL_WHITE);
    gl_draw_char(MONITOR_WIDTH / 2 - 200 + char_width * 1.5, MONITOR_HEIGHT * 0.47, name[1], GL_WHITE);
    gl_draw_char(MONITOR_WIDTH / 2 - 200 + char_width * 3.0, MONITOR_HEIGHT * 0.47, name[2], GL_WHITE);

    gl_swap_buffer();
    if(highscore_index != -1) {
        high_score_names[highscore_index] = malloc(4);
        for(int i = 0; i < 4; i++) {
            high_score_names[highscore_index][i] = name[i];
        }
    }
}
void letter_set_rotating_up() {
    rotating_up = true;
    rotating_down = false;
}
void letter_set_unrotating_up() {
    rotating_up = false;
}
void letter_set_rotating_down() {
    rotating_down = true;
    rotating_up = false;
}
void letter_set_unrotating_down() {
    rotating_down = false;
}
void letter_enter() {
    // Resets letter
    letter_selected = 'A'-1;
    // Advances to next index
    let_index++;

    // If the name has been fully entered, delay a half-second so user can see (disable interrupts temporarily), and then restart game.
    if(let_index == 3) {
        for(int i = 0; i < 3; i++) {
            if(name[i] == '_') name[i] = ' ';
        }
        unsigned long start = timer_get_ticks();
        while(timer_get_ticks() - start < 500000*TICKS_PER_USEC) {
            game_over_screen();
        }
        restart_game();
    }
}
