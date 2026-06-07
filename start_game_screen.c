#include "start_game_screen.h"
#include "game_over_screen.h"
#include "timer.h"
#include "rand.h"
#include "printf.h"

bool waiting = true;

void start_game_screen() {
    while (waiting) {
        gl_clear(GL_BLACK);

        gl_draw_string(MONITOR_WIDTH / 2 - 100, MONITOR_HEIGHT * 0.3, "Asteroids", GL_WHITE);
        gl_draw_string(MONITOR_WIDTH / 2 - 220, MONITOR_HEIGHT * 0.4, "Press any button to start", GL_WHITE);

        gl_draw_string(MONITOR_WIDTH / 2 - 115, MONITOR_HEIGHT * 0.45, "High Scores", GL_WHITE);

        char printbuf[20];
        for(int i = 0; i < 10; i++) {
            int score = high_scores[i];
            char* score_nm = high_score_names[i];
            if(score > -1) {
                snprintf(printbuf, 20, "%d. %d %c%c%c\n", i+1, score, score_nm[0], score_nm[1], score_nm[2]);
                gl_draw_string(MONITOR_WIDTH / 2 - 125, MONITOR_HEIGHT * (0.50 + ((double)i)*0.05), printbuf, GL_WHITE);
            }
        }

        gl_swap_buffer();
    }
    // Seeds random number generation upon start screen end.
    unsigned int ticks = (unsigned int)timer_get_ticks();
    srand(ticks);
}

