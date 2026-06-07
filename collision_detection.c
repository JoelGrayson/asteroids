#include "collision_detection.h"

static void check_if_bullet_touching_objs(struct bullet **bullet) {
    // # Bullet <-> Asteroid
    struct asteroid_list_t *asteroid_to_explode = get_asteroid_collision((*bullet)->mechanics, BULLET_COLLISION_POINTS, BULLET_NUM_POINTS);
    if (asteroid_to_explode != NULL) {
        // If an asteroid has collided with our bullet

        if ((*bullet)->owner == ROCKET) {
            // Only increase score if rocket destroyed it, not saucer
            asteroid_increase_score_by(asteroid_to_explode->ast.size);
        }

        // Destroy asteroid and delete bullet
        asteroid_explode(asteroid_to_explode);
        delete_bullet(bullet);
    }


    // # Bullet <-> Rocket
    if((*bullet)->owner == SAUCER && !rocket_is_invincible()) {
        if (are_colliding((*bullet)->mechanics, get_rocket_mechanics(), BULLET_COLLISION_POINTS, rotated_rocket_points, BULLET_NUM_POINTS, ROCKET_NUM_POINTS)) {
            delete_bullet(bullet);
            rocket_explode();
        }
    }


    // # Bullet <-> Saucer
    if (saucer_state != NO_SAUCER) {
        if ((*bullet)->owner == ROCKET) { //only rocket bullet can destroy saucer
            if (are_colliding((*bullet)->mechanics, get_saucer_mechanics(), BULLET_COLLISION_POINTS, get_saucer_points(), BULLET_NUM_POINTS, get_num_saucer_exterior_points())) {
                // Bullet touching saucer
                delete_bullet(bullet);
                despawn_saucer(true);
                // 1000 - small, 200 big
                if (saucer_state == BIG_SAUCER) {
                    increase_score_by(200);
                }
                if (saucer_state == SMALL_SAUCER) {
                    increase_score_by(1000);
                }
            }
        }
    }
}

/*
Check for the following collisions between objects:
Bullet and asteroid
Bullet and rocket
Bullet and saucer
Rocket and asteroid
Rocket and saucer
*/
void collision_detection() {
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


    // Saucer collision asteroid

    // Rocket collision saucer
    
    if(!rocket_is_invincible()) {
        // # Rocket <-> Asteroid
        struct asteroid_list_t *asteroid_to_explode = get_asteroid_collision(get_rocket_mechanics(), rotated_rocket_points, ROCKET_NUM_POINTS);
        if (asteroid_to_explode != NULL) {
            //if an asteroid has collided with the rocket
            asteroid_explode(asteroid_to_explode);
            asteroid_increase_score_by(asteroid_to_explode->ast.size);
            rocket_explode();
        }
    }

    if (saucer_state != NO_SAUCER) {
        // # Saucer <-> Asteroid
        struct asteroid_list_t *asteroid_to_explode = get_asteroid_collision(get_saucer_mechanics(), get_saucer_points(), get_num_saucer_exterior_points());
        if (asteroid_to_explode != NULL) {
            asteroid_explode(asteroid_to_explode);
            despawn_saucer(true);
        } else {

            if (!rocket_is_invincible()) {
                // # Rocket <-> Saucer (saucer just destroys rocket but is not itself destroyed)

                if (are_colliding(get_rocket_mechanics(), get_saucer_mechanics(), rotated_rocket_points, get_saucer_points(), ROCKET_NUM_POINTS, get_num_saucer_exterior_points())) {
                    rocket_explode();
                }
            }
        }
    }
}

