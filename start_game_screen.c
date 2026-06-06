#include "start_game_screen.h"
#include "timer.h"
#include "rand.h"

bool waiting = true;

void start_game_screen() {
    gl_clear(GL_BLACK);

    gl_draw_string(MONITOR_WIDTH / 2 - 100, MONITOR_HEIGHT * 0.3, "Asteroids", GL_WHITE);
    gl_draw_string(MONITOR_WIDTH / 2 - 300, MONITOR_HEIGHT * 0.4, "Press any button to start", GL_WHITE);

    gl_swap_buffer();

    while (waiting) {
        printf("Waiting in start screen\n");
    }
    // Seeds random number generation upon start screen end.
    unsigned int ticks = (unsigned int)timer_get_ticks();
    srand(ticks);
}

