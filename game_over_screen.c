#include "start_game_screen.h"
#include "game_manager_state.h"

void game_over_screen() {
    gl_clear(GL_BLACK);

    gl_draw_string(MONITOR_WIDTH / 2 - 300, MONITOR_HEIGHT * 0.2, "Your score is one of the ten best", GL_WHITE);
    gl_draw_string(MONITOR_WIDTH / 2 - 300, MONITOR_HEIGHT * 0.25, "Please enter your initials", GL_WHITE);
    gl_draw_string(MONITOR_WIDTH / 2 - 300, MONITOR_HEIGHT * 0.30, "Push ROTATE to select letter", GL_WHITE);
    gl_draw_string(MONITOR_WIDTH / 2 - 300, MONITOR_HEIGHT * 0.35, "Push HYPERSPACE when letter is correct", GL_WHITE);

    gl_swap_buffer();
}

