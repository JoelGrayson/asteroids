/* File: audio_demo.c
 * ------------------
 * Demo I2S playback and capture
 */
#include "assert.h"
#include "../libmango/dma.h"
#include "../libmango/i2s.h"
#include "printf.h"
#include "timer.h"
#include "uart.h"
#include <stdint.h>

#include "../audio/sounds_wav.h" // header file contains all asteroids sounds converted from wav files.
#include "../audio/all-right-now.h"

static void pause(const char *msg) {
    printf("%s ", msg);
    while (!uart_haschar());
    uart_putchar(uart_getchar());
}

void main(void) {
    uart_init();
    dma_init();

    assert(gwav_fire->bits_per_sample == 16); // DAC support only tested for 16-bit
    int sample_freq = gwav_fire->frames_per_sec;
    i2s_init(sample_freq);
    i2s_frame_type_t ftype = (gwav_fire->samples_per_frame == 1) ? I2S_MONO : I2S_STEREO;

    for(int i = 0; i < 12818; i++) {
        int16_t *datum = (int16_t*)(gwav_fire->data)+i;
        *datum = *datum / 4;
    }
    while(1) {
        i2s_stream_playback_nb(gwav_fire->data, gwav_fire->n_data, ftype);
        i2s_await();
        timer_delay_ms(250);
    }
}
