#include "score_and_lives.h"
#include "strings.h"
#include "printf.h"
#include "rocket.h"

#define MAX_SCORE_STRING_SIZE 1024

static int num_lives = 4;
static int score = 0;
static char *score_string;

void score_and_lives_init() {
    score_string = malloc(MAX_SCORE_STRING_SIZE);
}

int get_num_lives() {
    return num_lives;
}

#define LEFT_OFFSET 100
#define ROCKET_WIDTH 40

static void render_score() {
    snprintf(score_string, MAX_SCORE_STRING_SIZE, "%d", score);
    gl_draw_string(LEFT_OFFSET, 100, score_string, GL_WHITE);
    
}

static void render_lives() {
    // Draw num_lives number of rockets
    for (int i = 0; i < num_lives; i++) {
        int x = i * ROCKET_WIDTH + LEFT_OFFSET;
        draw_points(ROCKET_POINTS_TEMPLATE, ROCKET_NUM_POINTS, x, 150, GL_WHITE);
    }
}

void render_score_and_lives() {
    render_score();
    render_lives();
}

void decrease_lives() {
    num_lives--;
}


