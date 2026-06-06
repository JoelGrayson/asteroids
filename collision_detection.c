#include "collision_detection.h"

static void check_if_bullet_touching_objs(struct bullet **bullet) {
    // # Bullet <-> Asteroid
    struct asteroid_list_t *asteroid_to_explode = get_asteroid_collision((*bullet)->mechanics, BULLET_COLLISION_POINTS, BULLET_NUM_POINTS);

    // If an asteroid has collided with our bullet
    if (asteroid_to_explode != NULL) {
        // Only increase score if rocket destroyed it, not saucer
        if ((*bullet)->owner == ROCKET) {
            asteroid_increase_score_by(asteroid_to_explode->ast.size);
        }

        // Destroy asteroid and delete bullet
        asteroid_explode(asteroid_to_explode);
        delete_bullet(bullet);
    }


    // # Bullet <-> Rocket
    if ((*bullet)->owner == SAUCER) { //only saucer bullet can destroy rocket
        if (are_colliding((*bullet)->mechanics, get_rocket_mechanics(), BULLET_COLLISION_POINTS, rotated_rocket_points, BULLET_NUM_POINTS, ROCKET_NUM_POINTS)) {
            // Bullet touching rocket
            delete_bullet(bullet);
            rocket_explode();
        }
    }


    // # Bullet <-> Saucer
    if ((*bullet)->owner == ROCKET) { //only rocket bullet can destroy saucer
        if (are_colliding((*bullet)->mechanics, get_saucer_mechanics(), BULLET_COLLISION_POINTS, get_saucer_points(), BULLET_NUM_POINTS, get_num_saucer_exterior_points())) {
            // Bullet touching saucer
            delete_bullet(bullet);
            despawn_saucer();
            // TODO: proper point increment count
            increase_score_by(100);
        }
    }


}

void collision_detection() {
    // Check for the following collisions between objects
    // Bullet and asteroid
    // Bullet and rocket
    // Bullet and saucer
    // Rocket and asteroid
    // Rocket and saucer

    // # Bullet collisions (with asteroid, rocket, and saucer)
    int num_remaining_bullets = get_curr_num_bullets();
    struct bullet **bullet;
    int i = 0;
    // Until we run out of allocated bullets to check collisions for, keep iterating to check collisions.
    while(num_remaining_bullets > 0) {
        bullet = &bullets[i];
        // If the bullet is actually allocated, check for collisions with asteroids:
        if (*bullet != NULL) {
            check_if_bullet_touching_objs(bullet);
            num_remaining_bullets--;
        }
        i++;
    }

    // Rocket collision Asteroid

    // Saucer collision asteroid

    // Rocket collision saucer (saucer just destroys rocket but is not itself destroyed)

    
    if(rocket_asteroid_collision()) {
        rocket_explode();
    }
}

