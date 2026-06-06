#include "sounds.h"

/*static unsigned long last_sound_tick;
static unsigned long last_sound_duration_ticks;
#define FIRE_SOUND_TICK_DURATION 400000*TICKS_PER_USEC
#define THRUST_SOUND_TICK_DURATION 400000*TICKS_PER_USEC
#define EXPLOSION_SOUND_TICK_DURATION 2000000*TICKS_PER_USEC

static rb_t* sounds_to_play;*/

static void attenuate_wav(const struct wav_file_t *gwav) {
    for(int i = 0; i < gwav->n_data; i++) {
        int16_t *datum = (int16_t*)(gwav->data)+i;
        *datum = *datum / AMP_ATTENUATION;
    }
}

// Function to play sounds for game!
void sounds_play() {
    /*if(!rb_empty(sounds_to_play) && timer_get_ticks()-last_sound_tick > last_sound_duration_ticks) {
        const struct wav_file_t *gwav = (const struct wav_file_t*)rb_dequeue();
        while(!rb_empty(sounds_to_play)) {
            rb_dequeue();
        }
        assert(gwav->bits_per_sample == 16); // DAC support only tested for 16-bit
        int sample_freq = gwav->frames_per_sec;
        i2s_frame_type_t ftype = (gwav->samples_per_frame == 1) ? I2S_MONO : I2S_STEREO;
        i2s_stream_playback_nb(gwav->data, gwav->n_data, ftype);
    }*/

    // TODO!
}

static void play_sound(const struct wav_file_t *gwav) {
    i2s_await(); //prevents glitch

    //rb_enqueue(sounds_to_play, (int)gwav);
    assert(gwav->bits_per_sample == 16); // DAC support only tested for 16-bit
    // int sample_freq = gwav->frames_per_sec;
    i2s_frame_type_t ftype = (gwav_fire->samples_per_frame == 1) ? I2S_MONO : I2S_STEREO;
    i2s_stream_playback_nb(gwav->data, gwav->n_data, ftype);
}

void sounds_init() {
    dma_init();
    i2s_init(48000); // All of our sounds are at 48 kHz!
    //sounds_to_play = rb_new();
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
    play_sound(gwav_fire);
}
void play_thrust() {
    play_sound(gwav_thrust);
}
void play_beat1() {
    play_sound(gwav_beat1);
}
void play_beat2() {
    play_sound(gwav_beat2);
}
void play_bangSmall() {
    play_sound(gwav_bangSmall);
}
void play_bangMedium() {
    play_sound(gwav_bangMedium);
}
void play_bangLarge() {
    play_sound(gwav_bangLarge);
}
void play_saucerSmall() {
    play_sound(gwav_saucerSmall);
}
void play_saucerBig() {
    play_sound(gwav_saucerBig);
}
void play_extraShip() {
    play_sound(gwav_extraShip);
}
