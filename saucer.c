#include "saucer.h"

#define NUM_SAUCER_EXTERIOR_POINTS (sizeof(BIG_SAUCER_EXTERIOR_POINTS) / sizeof(BIG_SAUCER_EXTERIOR_POINTS[0]))
struct point BIG_SAUCER_EXTERIOR_POINTS[] = {
    { -6, -17 },
    { 4, -17 },
    { 8, -8 },
    { 21, 2 },
    { 10, 12 },
    { -10, 12 },
    { -21, 2 },
    { -10, -8 },
    { -6, -17 }
};
struct point BIG_SAUCER_TOP_LINE_POINTS[] = {
    { -10, -8 },
    { 8, -8 }
};
struct point BIG_SAUCER_BOTTOM_LINE_POINTS[] = {
    { -21, 2 },
    { 21, 2 }
};

#define SMALL_SAUCER_RATIO 0.7
struct point SMALL_SAUCER_EXTERIOR_POINTS[] = {
    { -6  * SMALL_SAUCER_RATIO, -17 * SMALL_SAUCER_RATIO },
    { 4   * SMALL_SAUCER_RATIO, -17 * SMALL_SAUCER_RATIO },
    { 8   * SMALL_SAUCER_RATIO, -8  * SMALL_SAUCER_RATIO },
    { 21  * SMALL_SAUCER_RATIO, 2   * SMALL_SAUCER_RATIO },
    { 10  * SMALL_SAUCER_RATIO, 12  * SMALL_SAUCER_RATIO },
    { -10 * SMALL_SAUCER_RATIO, 12  * SMALL_SAUCER_RATIO },
    { -21 * SMALL_SAUCER_RATIO, 2   * SMALL_SAUCER_RATIO },
    { -10 * SMALL_SAUCER_RATIO, -8  * SMALL_SAUCER_RATIO },
    { -6  * SMALL_SAUCER_RATIO, -17 * SMALL_SAUCER_RATIO }
};
struct point SMALL_SAUCER_TOP_LINE_POINTS[] = {
    { -10 * SMALL_SAUCER_RATIO, -8  * SMALL_SAUCER_RATIO },
    { 8   * SMALL_SAUCER_RATIO, -8 *  SMALL_SAUCER_RATIO }
};
struct point SMALL_SAUCER_BOTTOM_LINE_POINTS[] = {
    { -21 * SMALL_SAUCER_RATIO, 2  *  SMALL_SAUCER_RATIO },
    { 21  * SMALL_SAUCER_RATIO, 2 *   SMALL_SAUCER_RATIO }
};

static struct mechanics saucer_mechanics;
static enum saucer_state saucer_state;

void render_saucer() {
    if (saucer_state == NO_SAUCER) {

    } else if (saucer_state == BIG_SAUCER) {
        draw_points(BIG_SAUCER_EXTERIOR_POINTS, NUM_SAUCER_EXTERIOR_POINTS, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
        draw_points(BIG_SAUCER_TOP_LINE_POINTS, NUM_SAUCER_EXTERIOR_POINTS, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
        draw_points(BIG_SAUCER_BOTTOM_LINE_POINTS, NUM_SAUCER_EXTERIOR_POINTS, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
    } else if (saucer_state == SMALL_SAUCER) {
        draw_points(SMALL_SAUCER_EXTERIOR_POINTS, NUM_SAUCER_EXTERIOR_POINTS, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
        draw_points(SMALL_SAUCER_TOP_LINE_POINTS, NUM_SAUCER_EXTERIOR_POINTS, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
        draw_points(SMALL_SAUCER_BOTTOM_LINE_POINTS, NUM_SAUCER_EXTERIOR_POINTS, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
    } else {
        printf("Error: invalid saucer state %s\n", saucer_state);
    }
}


int frames_until_saucer_respawns = FPS * 10; //takes 10 seconds for a new saucer to spawn
static int frame_when_saucer_last_despawned = 0; //when saucer dies, set this to the frame number. If 


void saucer_frame_call(frame_number) {

}

