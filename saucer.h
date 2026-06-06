#include "graphics/point.h"
#include "graphics/draw_points.h"
#include "fps.h"
#include "mechanics.h"
#include <stdbool.h>
#include "printf.h"
#include "rand.h"
#include "constants.h"
#include "maths.h"

enum saucer_state {
    NO_SAUCER, //no saucer on screen
    SMALL_SAUCER, //the tiny saucer (harder) which shoots more accurately
    BIG_SAUCER //the clumsy big saucer
};

void setup_saucer();
void render_saucer();
/** Should be called once in each run_one_frame of game. Renders the saucer and decides whether to spawn another saucer */
void loop_saucer(long frame);

/*
Saucer moves around erratically. Every two seconds, it changes direction though at the same speed.

The way it shoots is random if big and at you if small.
Always once per second it shoots though
*/

