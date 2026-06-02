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
        }
        i++;
    }
}

