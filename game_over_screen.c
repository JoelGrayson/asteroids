#include "start_game_screen.h"

void game_over_screen() {
    gl_clear(GL_BLACK);

    gl_draw_string(MONITOR_WIDTH / 2 - 100, MONITOR_HEIGHT * 0.3, "Game over", GL_WHITE);
    gl_draw_string(MONITOR_WIDTH / 2 - 300, MONITOR_HEIGHT * 0.4, "Enter your name", GL_WHITE);
    gl_draw_string(MONITOR_WIDTH / 2 - 100, MONITOR_HEIGHT * 0.5, "___", GL_WHITE);
}

