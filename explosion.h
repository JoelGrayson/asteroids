#pragma once

#include "fps.h"
#include "graphics/point.h"

#define NUM_FRAMES_OF_EXPLOSION FPS //1 second

/** Draws an explosion (stardust) at position at frame number */
void render_explosion(struct point position, int frame_number);

