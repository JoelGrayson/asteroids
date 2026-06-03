#include "score_and_lives.h"
#include "strings.h"
#include "libmango/num_to_string.h"

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

static void render_score() {
    num_to_string(score, 10, score_string);
    gl_draw_string(0, 0, score_string, GL_WHITE);
}

static void render_lives() {
    
}

void render_score_and_lives() {
    
}


