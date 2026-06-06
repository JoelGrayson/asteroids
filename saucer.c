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
enum saucer_state saucer_state = NO_SAUCER;

static int num_frames_after_saucer_explode = 0;
static bool saucer_is_exploding = false;

const double big_saucer_bullet_speed = 4;
const double small_saucer_bullet_speed = 8;

void render_saucer() {
    if (saucer_state == NO_SAUCER) {
        if (saucer_is_exploding) {
            render_explosion(mechanics_to_point(saucer_mechanics), num_frames_after_saucer_explode);
        }
    } else if (saucer_state == BIG_SAUCER) {
        draw_points(BIG_SAUCER_EXTERIOR_POINTS, NUM_SAUCER_EXTERIOR_POINTS, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
        draw_points(BIG_SAUCER_TOP_LINE_POINTS, 2, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
        draw_points(BIG_SAUCER_BOTTOM_LINE_POINTS, 2, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
    } else if (saucer_state == SMALL_SAUCER) {
        draw_points(SMALL_SAUCER_EXTERIOR_POINTS, NUM_SAUCER_EXTERIOR_POINTS, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
        draw_points(SMALL_SAUCER_TOP_LINE_POINTS, 2, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
        draw_points(SMALL_SAUCER_BOTTOM_LINE_POINTS, 2, saucer_mechanics.x, saucer_mechanics.y, GL_WHITE);
    } else {
        printf("Error: invalid saucer state %d\n", saucer_state);
    }
}


int frames_until_saucer_respawns = FPS * 10; //takes 10 seconds for a new saucer to spawn
static long frame_when_saucer_last_spawned = 0; //when saucer dies, set this to the frame number. If 
static long frame_when_saucer_last_despawned = 0; //when saucer dies, set this to the frame number. If 


void setup_saucer() {
    saucer_mechanics.x = 0;
    saucer_mechanics.y = 0;
    saucer_mechanics.vx = 0;
    saucer_mechanics.vy = 0;
    saucer_mechanics.ax = 0;
    saucer_mechanics.ay = 0;
    saucer_mechanics.rotation = 0;

    saucer_state = NO_SAUCER;
}

static double saucer_speed = 5.0;

static void spawn_saucer(enum saucer_state new_saucer_state) {
    if (new_saucer_state == BIG_SAUCER) {
        printf("Spawning big saucer\n");
        saucer_state = new_saucer_state;
    }
    if (new_saucer_state == SMALL_SAUCER) {
        printf("Spawning smol saucer\n");
        saucer_state = new_saucer_state;
    }

    int corner_where_saucer_spawns = 0;// rand() % 4; //0 means N, 1 means E, 2 means S, 3 means W
    switch (corner_where_saucer_spawns) {
        case 0: //top
            saucer_mechanics.x = rand() % (int)MONITOR_WIDTH;
            saucer_mechanics.y = 0;
            saucer_mechanics.vy = saucer_speed;
            saucer_mechanics.vx = 0;
            break;
        case 1: //right
            break;
        case 2: //left
            break;
        case 3: //right
            break;
        default:
            printf("Error: undefined corner\n");
    }
}

static double random_angle();
static void face_toward(double angle);

static double get_saucer_width() {
    if (saucer_state == BIG_SAUCER) {
        return 42;
    }
    return 42 * SMALL_SAUCER_RATIO;
}

static double get_saucer_height() {
    if (saucer_state == BIG_SAUCER) {
        return 29;
    }
    return 29 * SMALL_SAUCER_RATIO;
}

void loop_saucer() {
    render_saucer();

    if (saucer_state == NO_SAUCER) {
        // Spawn saucer if no saucer and it's been a while
        // printf("frame=%10ld, frame_when_saucer_last_despawned=%10ld, diff=%10ld, until=%10d\n", frame, frame_when_saucer_last_despawned, (frame - frame_when_saucer_last_despawned), frames_until_saucer_respawns);

        if (saucer_is_exploding) {
            num_frames_after_saucer_explode++;
        }
        
        if ((frame - frame_when_saucer_last_despawned) > frames_until_saucer_respawns) {
            // spawn saucer
            frame_when_saucer_last_spawned = frame;
            spawn_saucer(SMALL_SAUCER);
            // if (rand() % 2 == 0) { //50-50 chance of being big or small
            //     spawn_saucer(BIG_SAUCER);
            // } else {
            //     spawn_saucer(SMALL_SAUCER);
            // }
        }
    } else {
        // There is a saucer  

        int saucer_frame = frame - frame_when_saucer_last_spawned;


        if (saucer_state == BIG_SAUCER) {
            // Big saucer is clumsy. Just shoots once per second and it is in a random direction

            // Move in random direction every 3 seconds after 3 seconds passed (to make it toward the center of the screen)
            if (saucer_frame >= FPS * 3 && saucer_frame % (int)(FPS * 3) == 0) {
                face_toward(random_angle());
            }

            if (saucer_frame % (int)FPS == 0) { //every second
                // Shoot!
                double angle = random_angle();

                struct mechanics new_bullet_mechanics = {
                    .x = saucer_mechanics.x,
                    .y = saucer_mechanics.y,
                    .vx = big_saucer_bullet_speed * cosine(angle),
                    .vy = big_saucer_bullet_speed * sine(angle),
                    .ax = 0,
                    .ay = 0,
                    .rotation = 0
                };
                new_bullet(new_bullet_mechanics, SAUCER);
            }
        } else {
            // Small saucer shoots at you with a random offset and shoots twice per second. The bullets are faster too.

            // Move in random direction every 1.5 seconds after 3 seconds passed (to make it toward the center of the screen)
            if (saucer_frame >= FPS * 3 && saucer_frame % (int)(FPS * 1.5) == 0) {
                face_toward(random_angle());
            }

            if (saucer_frame % (int)(FPS / 2) == 0) { //twice a second
                // Shoot!
                double angle = angle_from(mechanics_to_point(saucer_mechanics), mechanics_to_point(get_rocket_mechanics()));
                printf("angle from saucer→rocket = %d / 100\n", (int)(angle*100));

                struct mechanics new_bullet_mechanics = {
                    .x = saucer_mechanics.x,
                    .y = saucer_mechanics.y,
                    .vx = small_saucer_bullet_speed * cosine(angle),
                    .vy = small_saucer_bullet_speed * sine(angle),
                    .ax = 0,
                    .ay = 0,
                    .rotation = 0
                };
                new_bullet(new_bullet_mechanics, SAUCER);
            }
        }

        update_mechanics(&saucer_mechanics, false);
        if (out_of_bounds(mechanics_to_point(saucer_mechanics), get_saucer_width(), get_saucer_height())) {
            // printf("Saucer out of bounds\n");
            despawn_saucer(false);
        }
    }
}

/** @returns 0 to 2pi */
static double random_angle() {
    return (rand() % 628) / 100;
}

static void face_toward(double angle) {
    saucer_mechanics.vx = saucer_speed * cosine(angle);
    saucer_mechanics.vy = saucer_speed * sine(angle);
}

void despawn_saucer(bool explode) {
    saucer_state = NO_SAUCER; //remove saucer
    frame_when_saucer_last_despawned = frame;

    if (explode) {
        num_frames_after_saucer_explode = 0;
        saucer_is_exploding = true;
    }

    // TODO: add sound effect when saucer destroyed
}

struct mechanics get_saucer_mechanics() {
    return saucer_mechanics;
}

struct point *get_saucer_points() {
    switch (saucer_state) {
        case SMALL_SAUCER:
            return SMALL_SAUCER_EXTERIOR_POINTS;
        case BIG_SAUCER:
            return BIG_SAUCER_EXTERIOR_POINTS;
        case NO_SAUCER:
            return NULL;
        default:
            return NULL;
    }
}

int get_num_saucer_exterior_points() {
    return NUM_SAUCER_EXTERIOR_POINTS;
}

