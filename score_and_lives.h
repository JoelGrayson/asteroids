#pragma once

#include "gl.h"
#include "malloc.h"
#include "graphics/draw_points.h"

void score_and_lives_init();

int get_num_lives();
int get_score();
void increase_score_by(int);

void loop_score_and_lives();
void decrease_lives();

