#include "graphics/point.h"
#include "graphics/draw_points.h"
#include "fps.h"
#include "mechanics.h"
#include <stdbool.h>
#include "printf.h"

enum saucer_state {
    NO_SAUCER, //no saucer on screen
    SMALL_SAUCER, //the tiny saucer (harder) which shoots more accurately
    BIG_SAUCER //the clumsy big saucer
};

void render_saucer();
void saucer_frame_call(frame_number); //renders the saucer and decides whether to spawn another saucer

