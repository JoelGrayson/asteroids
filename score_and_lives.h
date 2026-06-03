#pragma once

#include "gl.h"
#include "malloc.h"
#include "graphics/draw_points.h"

void score_and_lives_init();
int get_num_lives();
void render_score_and_lives();
void decrease_lives();

