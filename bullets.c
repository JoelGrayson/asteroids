#include "bullets.h"
#include <stdbool.h>

struct bullet *bullets[MAX_NUM_BULLETS] = { 0 }; //zero out so all are NULL

// A square. More visible than a single point
static struct point BULLET_POINTS[BULLET_NUM_POINTS] = {
    { 0, 0 },
    { 1, 0 },
    { 1, 1 },
    { 0, 1 }
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

void draw_bullets() {
    for (int i = 0; i < MAX_NUM_BULLETS; i++) {
        struct bullet *bullet = bullets[i];
        if (bullet) {
            draw_bullet(bullet);
        }
    }
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
        if (bullet && out_of_bounds((struct point){ .x = bullet->mechanics.x, .y = bullet->mechanics.y })) {
            free(bullet);
            bullets[i] = NULL;
        }
    }
}

struct point *get_bullet_points() {
    return BULLET_POINTS;
}

