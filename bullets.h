#include "mechanics.h"
#include "printf.h"
#include "gl.h"
#include "malloc.h"
#include "graphics/out_of_bounds.h"

/* Represents who shot the bullet */
#define MAX_NUM_BULLETS 1024

enum bullet_owner {
    ROCKET,
    SAUCER
};

// This is not a template. No rotation rendering on bullet
#define BULLET_NUM_POINTS 4


/* Represents a bullet */
struct bullet {
    enum bullet_owner owner;
    struct mechanics mechanics;
};

/* Creates a new bullet with specified mechanics and owner */
void new_bullet(struct mechanics mechanics, enum bullet_owner owner);

// Extern of list of pointers to bullets. Each element of list is either NULL (no bullet) or a pointer to a bullet
// Use extern pattern instead of get_bullets because then it would copy
extern struct bullet *bullets[MAX_NUM_BULLETS];

void draw_bullet(struct bullet *bullet) ;

void draw_bullets();

void bullets_update_mechanics();

struct point *get_bullet_points();

