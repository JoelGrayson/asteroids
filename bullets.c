#include "bullets.h"
#include "asteroid.h"
#include "printf.h"
#include <stdbool.h>

struct bullet *bullets[MAX_NUM_BULLETS] = { 0 }; //zero out so all are NULL
static int curr_num_bullets = 0; // number of allocated bullets starts out at zero

// A square. More visible than a single point
static struct point BULLET_POINTS[BULLET_NUM_POINTS] = {
    { -0.5, -0.5 },
    { 0.5, -0.5 },
    { 0.5, 0.5 },
    { -0.5, 0.5 }
};

static struct point BULLET_COLLISION_POINTS[4] = {
    { -5, -5 },
    {  5, -5 },
    { -5,  5 },
    {  5,  5 }
};

void new_bullet(struct mechanics mechanics, enum bullet_owner owner) {
    int i = 0;
    while (true) {
        if (i == MAX_NUM_BULLETS) {
            printf("There is no more room to allocate a bullet\n");
            return;
        }
        if (bullets[i] == NULL) {
            // Found a blank place to insert a new bullet
            bullets[i] = malloc(sizeof(struct bullet));
            bullets[i]->mechanics = mechanics;
            bullets[i]->owner = owner;
            curr_num_bullets++; // Increase total count of allocated bullets
            return;
        }
        i++;
    }
}


void draw_bullet(struct bullet *bullet) {
    struct mechanics mech = bullet->mechanics;
    
    gl_draw_pixel(mech.x, mech.y, GL_WHITE);
    gl_draw_pixel(mech.x, mech.y + 1, GL_WHITE);
    gl_draw_pixel(mech.x + 1, mech.y, GL_WHITE);
    gl_draw_pixel(mech.x + 1, mech.y + 1, GL_WHITE);
}

void render_bullets() {
    for (int i = 0; i < MAX_NUM_BULLETS; i++) {
        struct bullet *bullet = bullets[i];
        if (bullet) {
            draw_bullet(bullet);
        }
    }
}

void bullets_update_mechanics();
void loop_bullets() {
    render_bullets();
    bullets_update_mechanics();
}

void clean_up_out_of_bounds_bullets();
void bullets_update_mechanics() {
    for (int i = 0; i < MAX_NUM_BULLETS; i++) {
        struct bullet *bullet = bullets[i];
        if (bullet) {
            update_mechanics(&bullet->mechanics, false);
        }
    }

    clean_up_out_of_bounds_bullets();
}

/* Deallocates bullets who have gone off the screen. Called manually by bullet_update_mechanics */
void clean_up_out_of_bounds_bullets() {
    for (int i = 0; i < MAX_NUM_BULLETS; i++) {
        struct bullet *bullet = bullets[i];
        if (bullet && out_of_bounds(mechanics_to_point(bullet->mechanics))) {
            free(bullet);
            bullets[i] = NULL;
            curr_num_bullets--; // Decrease total count of allocated bullets
        }
    }
}

void delete_bullet(struct bullet **bullet) {
    free(*bullet);
    *bullet = NULL;
    curr_num_bullets--; // Decrease total count of allocated bullets
}

struct point *get_bullet_points() {
    return BULLET_POINTS;
}

void bullets_asteroid_collision() {
    int num_remaining_bullets = curr_num_bullets; //number of remaining bullets to check
    struct bullet **bullet;
    int i = 0;

    // Until we run out of allocated bullets to check collisions for, keep iterating to check collisions.
    while(num_remaining_bullets > 0) {
        bullet = &bullets[i];
        // If the bullet is actually allocated, check for collisions with asteroids:
        if(*bullet != NULL) {
            if((*bullet)->owner == ROCKET) {
                // Tries to find an asteroid which has collided with our bullet:
                struct asteroid_list_t *to_explode = get_asteroid_collision((*bullet)->mechanics, BULLET_COLLISION_POINTS, 4);
                // If an asteroid has collided with our bullet, remove it (explode it!) alongside our bullet, and then return true (there has been a collision).
                if(to_explode != NULL) {
                    asteroid_explode(to_explode);
                    delete_bullet(bullet);
                }
            }
            num_remaining_bullets--;
        }
        i++;
    }
}
