#include "assert.h"
#include "../libmango/dma.h"
#include "../libmango/i2s.h"
#include "sounds.h"
#include "sounds_wav.h"
#include <stdint.h>

static void attenuate_wav(const struct wav_file_t *gwav) {
    for(int i = 0; i < gwav->n_data; i++) {
        int16_t *datum = (int16_t*)(gwav->data)+i;
        *datum = *datum / AMP_ATTENUATION;
    }
}

// Function to play sounds for game!
static void play_sound(const struct wav_file_t *gwav) {
    assert(gwav->bits_per_sample == 16); // DAC support only tested for 16-bit
    // int sample_freq = gwav->frames_per_sec;
    i2s_frame_type_t ftype = (gwav_fire->samples_per_frame == 1) ? I2S_MONO : I2S_STEREO;
    i2s_stream_playback_nb(gwav->data, gwav->n_data, ftype);
}

void sounds_init() {
    dma_init();
    i2s_init(48000); // All of our sounds are at 48 kHz!
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
