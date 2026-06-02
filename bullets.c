#include "bullets.h"
#include <stdbool.h>

struct bullet *bullets[MAX_NUM_BULLETS] = { 0 }; //zero out so all are NULL

void new_bullet(struct mechanics mechanics, enum bullet_owner owner) {
    int i = 0;
    while (true) {
        if (i == MAX_NUM_BULLETS) {
            printf("There is no more room to allocate a bullet");
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
}

void draw_bullets() {
    for (int i = 0; i < MAX_NUM_BULLETS; i++) {
        struct bullet *bullet = bullets[i];
        if (bullet) {
            draw_bullet(bullet);
        }
    }
}

void bullets_update_mechanics() {
    for (int i = 0; i < MAX_NUM_BULLETS; i++) {
        struct bullet *bullet = bullets[i];
        if (bullet) {
            update_mechanics(&bullet->mechanics, false);
        }
    }
}
