I2S asynchronous audio in audio/sounds.c programmed by Sebastian (using Julie's I2S driver from lecture code, and using AI to refactor 11000 kHz, 8-bit-per-sample asteroids sound .wav files to 16-bit-per-sample 48 kHZ .wavs that could play on our audio chips).

Trigonometry functions (sine, cosine, tan, trig_init) in maths.c programmed by Sebastian

Point rotation calculations in graphics/rotate_points.c programmed by Sebastian Vector manipulation and line segment intersect checker (bool lines_intersect()) in graphics/geometry.c programmed by Sebastian.

Game over screen + high score tracking programmed by Sebastian in game_over_screen.c, later refactored by Joel. Asteroid spawning, collision detection, and motion programmed by Sebastian in asteroid.c -- later refactored by Joel. Joel also implemented the asteroid explosion animation.

Leaderboard in start screen programmed by Sebastian in main.c -- Joel later refactored that code to start_game_screen.c. Object collision check function bool are_colliding() in mechanics.c programmed by Sebastian. update_mechanics jointly programmed by Sebastian (base functionality) and Joel (screen wrap-around option).

Button handling in buttons.c programmed by Sebastian, later refactored by Joel somewhat. rocket_update_mechanics() rocket motion logic and rocket_asteroid_collision() rocket collision logic programmed by Sebastian -- most of rest of rocket.c programmed by Joel.

Game music beat (beat1 followed by beat2) programmed by Sebastian in main.c. The structure of the top-level program execution in the rest of main.c was programmed by Joel.

Joel programmed the saucers' functionalities in saucer.c, such as the small saucer targeting the player with focused bullets. Joel programmed the rocket bullet firing in rocket.c and the bullet motion and collision handling in bullets.c.

Joel programmed all object rendering functions (rocket_render(), asteroid_render, render_bullets(), et cetera, alongside manually inputting the points used to draw the perimeters of said objects). Joel programmed the rocket explosion animation, as well as that of the asteroid and saucer.

Joel programmed the line-drawing function draw_line() in graphics/draw_line.c Joel programmed the object perimeter-drawing function draw_points() in graphics/draw_points.c, which is used to draw every single object rendered in the asteroids game. Incidentally, this function directly employed the draw_line() function Joel previously implemented.

Joel programmed the score tracker and graphics for score and extra lives in score_and_lives.c. Joel programmed the SPI flash implementation for storing high scores in persistent memory to save them while the Asteroids game is de-powered (in spi_flash.c).

Joel did soldering and hot-gluing for final arcade console construction; Sebastian did woodcutting, drilling for casing. Design of arcade from contributions of both.
SD card boot implemented by Sebastian.


* Other people's code
  * [Audio](https://github.com/cs107e/cs107e.github.io/tree/master/lectures/Output/code/i2s_digital_audio)
  * [SD card boot](https://github.com/cs107e/sdcard_boot_spl)
