#include "sounds.h"
#include "sounds_wav.h"
#include "ringbuffer.h"
#include "timer.h"
#include <stdint.h>
#include "strings.h"
#include "../fps.h"

static unsigned long last_sound_tick;
static int16_t cur_buf[SAMP_PER_FRAME];

#define NUM_SIMULTANEOUS_SAME_SOUND 2
static int fire_frames[NUM_SIMULTANEOUS_SAME_SOUND];
static int thrust_frames[NUM_SIMULTANEOUS_SAME_SOUND];
static int beat1_frames[NUM_SIMULTANEOUS_SAME_SOUND];
static int beat2_frames[NUM_SIMULTANEOUS_SAME_SOUND];
static int bangSmall_frames[NUM_SIMULTANEOUS_SAME_SOUND];
static int bangMedium_frames[NUM_SIMULTANEOUS_SAME_SOUND];
static int bangLarge_frames[NUM_SIMULTANEOUS_SAME_SOUND];
static int saucerSmall_frames[NUM_SIMULTANEOUS_SAME_SOUND];
static int saucerBig_frames[NUM_SIMULTANEOUS_SAME_SOUND];
static int extraShip_frames[NUM_SIMULTANEOUS_SAME_SOUND];

static void attenuate_wav(const struct wav_file_t *gwav) {
    for(int i = 0; i < gwav->n_data; i++) {
        int16_t *datum = (int16_t*)(gwav->data)+i;
        *datum = *datum / AMP_ATTENUATION;
    }
}

// Function to play sounds for game!
void sounds_play() {
    // Ensures sound buffer defaults to no sound
    memset(cur_buf, 0, SAMP_PER_FRAME*2);

    int16_t att_factor = 0; // Attenuation factor for overall sound playback (we are overlapping multiple sounds here!)
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(fire_frames[i] != -1) att_factor++;
        if(thrust_frames[i] != -1) att_factor++;
        if(beat1_frames[i] != -1) att_factor++;
        if(beat2_frames[i] != -1) att_factor++;
        if(bangSmall_frames[i] != -1) att_factor++;
        if(bangMedium_frames[i] != -1) att_factor++;
        if(bangLarge_frames[i] != -1) att_factor++;
        if(saucerSmall_frames[i] != -1) att_factor++;
        if(saucerBig_frames[i] != -1) att_factor++;
        if(extraShip_frames[i] != -1) att_factor++;
    }
    if(att_factor != 0) { // if there are sounds to play, play them!
        for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
            // Copies over attenuated sample amplitude data to cur_buf from all playing sounds (2 per type of sound):
            int16_t* dat_to_copy;
            int nsamples;
            if(fire_frames[i] != -1) {
                // Finds the specific SAMP_PER_FRAME long block of samples we want to play back from the sound we are playing, gets an int16_t pointer to it.
                dat_to_copy = ((int16_t*)gwav_fire->data)+fire_frames[i];

                // Calculates the actual number of samples left (<= SAMP_PER_FRAME) that we can play of the sound during this frame.
                nsamples = (SAMP_PER_FRAME < (gwav_fire->n_data - fire_frames[i])) ? SAMP_PER_FRAME : gwav_fire->n_data - fire_frames[i];

                // Copies over said samples into sound playback buffer (cur_buf), with attenuation to avoid clipping/integer overflow effects.
                for(int j = 0; j < nsamples; j++) {
                    cur_buf[j] += dat_to_copy[j]/att_factor;
                }
                // Increments to next SAMP_PER_FRAME samples of sound if possible, if not deactivates sound.
                fire_frames[i] = (fire_frames[i] < gwav_fire->n_data) ? fire_frames[i] + SAMP_PER_FRAME : -1;
            }
            if(thrust_frames[i] != -1) {
                dat_to_copy = ((int16_t*)gwav_thrust->data)+thrust_frames[i];
                nsamples = (SAMP_PER_FRAME < (gwav_thrust->n_data - thrust_frames[i])) ? SAMP_PER_FRAME : gwav_thrust->n_data - thrust_frames[i];
                for(int j = 0; j < nsamples; j++) {
                    cur_buf[j] += dat_to_copy[j]/att_factor;
                }
                // Increments to next SAMP_PER_FRAME samples of sound if possible, if not deactivates sound.
                thrust_frames[i] = (thrust_frames[i] < gwav_thrust->n_data) ? thrust_frames[i] + SAMP_PER_FRAME : -1;
            }
            if(beat1_frames[i] != -1) {
                dat_to_copy = ((int16_t*)gwav_beat1->data)+beat1_frames[i];
                nsamples = (SAMP_PER_FRAME < (gwav_beat1->n_data - beat1_frames[i])) ? SAMP_PER_FRAME : gwav_beat1->n_data - beat1_frames[i];
                for(int j = 0; j < nsamples; j++) {
                    cur_buf[j] += dat_to_copy[j]/att_factor;
                }
                // Increments to next SAMP_PER_FRAME samples of sound if possible, if not deactivates sound.
                beat1_frames[i] = (beat1_frames[i] < gwav_beat1->n_data) ? beat1_frames[i] + SAMP_PER_FRAME : -1;
            }
            if(beat2_frames[i] != -1) {
                dat_to_copy = ((int16_t*)gwav_beat2->data)+beat2_frames[i];
                nsamples = (SAMP_PER_FRAME < (gwav_beat2->n_data - beat2_frames[i])) ? SAMP_PER_FRAME : gwav_beat2->n_data - beat2_frames[i];
                for(int j = 0; j < nsamples; j++) {
                    cur_buf[j] += dat_to_copy[j]/att_factor;
                }
                // Increments to next SAMP_PER_FRAME samples of sound if possible, if not deactivates sound.
                beat2_frames[i] = (beat2_frames[i] < gwav_beat2->n_data) ? beat2_frames[i] + SAMP_PER_FRAME : -1;
            }
            if(bangSmall_frames[i] != -1) {
                dat_to_copy = ((int16_t*)gwav_bangSmall->data)+bangSmall_frames[i];
                nsamples = (SAMP_PER_FRAME < (gwav_bangSmall->n_data - bangSmall_frames[i])) ? SAMP_PER_FRAME : gwav_bangSmall->n_data - bangSmall_frames[i];
                for(int j = 0; j < nsamples; j++) {
                    cur_buf[j] += dat_to_copy[j]/att_factor;
                }
                // Increments to next SAMP_PER_FRAME samples of sound if possible, if not deactivates sound.
                bangSmall_frames[i] = (bangSmall_frames[i] < gwav_bangSmall->n_data) ? bangSmall_frames[i] + SAMP_PER_FRAME : -1;
            }
            if(bangMedium_frames[i] != -1) {
                dat_to_copy = ((int16_t*)gwav_bangMedium->data)+bangMedium_frames[i];
                nsamples = (SAMP_PER_FRAME < (gwav_bangMedium->n_data - bangMedium_frames[i])) ? SAMP_PER_FRAME : gwav_bangMedium->n_data - bangMedium_frames[i];
                for(int j = 0; j < nsamples; j++) {
                    cur_buf[j] += dat_to_copy[j]/att_factor;
                }
                // Increments to next SAMP_PER_FRAME samples of sound if possible, if not deactivates sound.
                bangMedium_frames[i] = (bangMedium_frames[i] < gwav_bangMedium->n_data) ? bangMedium_frames[i] + SAMP_PER_FRAME : -1;
            }
            if(bangLarge_frames[i] != -1) {
                dat_to_copy = ((int16_t*)gwav_bangLarge->data)+bangLarge_frames[i];
                nsamples = (SAMP_PER_FRAME < (gwav_bangLarge->n_data - bangLarge_frames[i])) ? SAMP_PER_FRAME : gwav_bangLarge->n_data - bangLarge_frames[i];
                for(int j = 0; j < nsamples; j++) {
                    cur_buf[j] += dat_to_copy[j]/att_factor;
                }
                // Increments to next SAMP_PER_FRAME samples of sound if possible, if not deactivates sound.
                bangLarge_frames[i] = (bangLarge_frames[i] < gwav_bangLarge->n_data) ? bangLarge_frames[i] + SAMP_PER_FRAME : -1;
            }
            if(saucerSmall_frames[i] != -1) {
                dat_to_copy = ((int16_t*)gwav_saucerSmall->data)+saucerSmall_frames[i];
                nsamples = (SAMP_PER_FRAME < (gwav_saucerSmall->n_data - saucerSmall_frames[i])) ? SAMP_PER_FRAME : gwav_saucerSmall->n_data - saucerSmall_frames[i];
                for(int j = 0; j < nsamples; j++) {
                    cur_buf[j] += dat_to_copy[j]/att_factor;
                }
                // Increments to next SAMP_PER_FRAME samples of sound if possible, if not deactivates sound.
                saucerSmall_frames[i] = (saucerSmall_frames[i] < gwav_saucerSmall->n_data) ? saucerSmall_frames[i] + SAMP_PER_FRAME : -1;
            }
            if(saucerBig_frames[i] != -1) {
                dat_to_copy = ((int16_t*)gwav_saucerBig->data)+saucerBig_frames[i];
                nsamples = (SAMP_PER_FRAME < (gwav_saucerBig->n_data - saucerBig_frames[i])) ? SAMP_PER_FRAME : gwav_saucerBig->n_data - saucerBig_frames[i];
                for(int j = 0; j < nsamples; j++) {
                    cur_buf[j] += dat_to_copy[j]/att_factor;
                }
                // Increments to next SAMP_PER_FRAME samples of sound if possible, if not deactivates sound.
                saucerBig_frames[i] = (saucerBig_frames[i] < gwav_saucerBig->n_data) ? saucerBig_frames[i] + SAMP_PER_FRAME : -1;
            }
            if(extraShip_frames[i] != -1) {
                dat_to_copy = ((int16_t*)gwav_extraShip->data)+extraShip_frames[i];
                nsamples = (SAMP_PER_FRAME < (gwav_extraShip->n_data - extraShip_frames[i])) ? SAMP_PER_FRAME : gwav_extraShip->n_data - extraShip_frames[i];
                for(int j = 0; j < nsamples; j++) {
                    cur_buf[j] += dat_to_copy[j]/att_factor;
                }
                // Increments to next SAMP_PER_FRAME samples of sound if possible, if not deactivates sound.
                extraShip_frames[i] = (extraShip_frames[i] < gwav_extraShip->n_data) ? extraShip_frames[i] + SAMP_PER_FRAME : -1;
            }
        }
        while(timer_get_ticks() < last_sound_tick + TICKS_PER_USEC*1000000/FPS) { /* SPIN! */ } // Makes sure timing is ABSOLUTELY CORRECT for our playback!
        i2s_stream_playback_nb(cur_buf, SAMP_PER_FRAME, I2S_MONO); // Plays sounds in sound buffer through I2S!
        last_sound_tick = timer_get_ticks();
        //swap_sound_buf(); // Switches sound buffer to next frame's!

    }
}

static void play_sound(const struct wav_file_t *gwav) {
    assert(gwav->bits_per_sample == 16); // DAC support only tested for 16-bit
    // int sample_freq = gwav->frames_per_sec;
    i2s_frame_type_t ftype = (gwav_fire->samples_per_frame == 1) ? I2S_MONO : I2S_STEREO;
    i2s_stream_playback_nb(gwav->data, gwav->n_data, ftype);
}

void sounds_init() {
    dma_init();
    i2s_init(48000); // All of our sounds are at 48 kHz!
    // Set the status of the all sound playback location/timing trackers to "not playing" (-1).
    memset(fire_frames, -1, NUM_SIMULTANEOUS_SAME_SOUND*4);
    memset(thrust_frames, -1, NUM_SIMULTANEOUS_SAME_SOUND*4);
    memset(beat1_frames, -1, NUM_SIMULTANEOUS_SAME_SOUND*4);
    memset(beat2_frames, -1, NUM_SIMULTANEOUS_SAME_SOUND*4);
    memset(bangSmall_frames, -1, NUM_SIMULTANEOUS_SAME_SOUND*4);
    memset(bangMedium_frames, -1, NUM_SIMULTANEOUS_SAME_SOUND*4);
    memset(bangLarge_frames, -1, NUM_SIMULTANEOUS_SAME_SOUND*4);
    memset(saucerSmall_frames, -1, NUM_SIMULTANEOUS_SAME_SOUND*4);
    memset(saucerBig_frames, -1, NUM_SIMULTANEOUS_SAME_SOUND*4);
    memset(extraShip_frames, -1, NUM_SIMULTANEOUS_SAME_SOUND*4);

    // Ensures sound buffer defaults to no sound
    memset(cur_buf, 0, SAMP_PER_FRAME*2);

    attenuate_wav(gwav_fire);
    attenuate_wav(gwav_thrust);
    attenuate_wav(gwav_beat1);
    attenuate_wav(gwav_beat2);
    attenuate_wav(gwav_bangSmall);
    attenuate_wav(gwav_bangMedium);
    attenuate_wav(gwav_bangLarge);
    attenuate_wav(gwav_saucerSmall);
    attenuate_wav(gwav_saucerBig);
    attenuate_wav(gwav_extraShip);
}

void play_fire() {
    // If there is an available playback slot, play fire sound.
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(fire_frames[i] == -1) {
            fire_frames[i] = 0;
            return;
        }
    }
    //play_sound(gwav_fire);
}
void play_thrust() {
    // If there is an available playback slot, play thrust sound.
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(thrust_frames[i] == -1) {
            thrust_frames[i] = 0;
            return;
        }
    }
    //play_sound(gwav_thrust);
}
void play_beat1() {
    // If there is an available playback slot, play beat1 sound.
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(beat1_frames[i] == -1) {
            beat1_frames[i] = 0;
            return;
        }
    }
    //play_sound(gwav_beat1);
}
void play_beat2() {
    // If there is an available playback slot, play beat2 sound.
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(beat2_frames[i] == -1) {
            beat2_frames[i] = 0;
            return;
        }
    }
    //play_sound(gwav_beat2);
}
void play_bangSmall() {
    // If there is an available playback slot, play bangSmall sound.
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(bangSmall_frames[i] == -1) {
            bangSmall_frames[i] = 0;
            return;
        }
    }
    //play_sound(gwav_bangSmall);
}
void play_bangMedium() {
    // If there is an available playback slot, play bangMedium sound.
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(bangMedium_frames[i] == -1) {
            bangMedium_frames[i] = 0;
            return;
        }
    }
    //play_sound(gwav_bangMedium);
}
void play_bangLarge() {
    // If there is an available playback slot, play bangLarge sound.
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(bangLarge_frames[i] == -1) {
            bangLarge_frames[i] = 0;
            return;
        }
    }
    //play_sound(gwav_bangLarge);
}
void play_saucerSmall() {
    // If there is an available playback slot, play saucerSmall sound.
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(saucerSmall_frames[i] == -1) {
            saucerSmall_frames[i] = 0;
            return;
        }
    }
    //play_sound(gwav_saucerSmall);
}
void play_saucerBig() {
    // If there is an available playback slot, play saucerBig sound.
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(saucerBig_frames[i] == -1) {
            saucerBig_frames[i] = 0;
            return;
        }
    }
    //play_sound(gwav_saucerBig);
}
void play_extraShip() {
    // If there is an available playback slot, play extraShip sound.
    for(int i = 0; i < NUM_SIMULTANEOUS_SAME_SOUND; i++) {
        if(extraShip_frames[i] == -1) {
            extraShip_frames[i] = 0;
            return;
        }
    }
    //play_sound(gwav_extraShip);
}
