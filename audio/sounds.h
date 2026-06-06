#pragma once

#include "assert.h"
#include "../libmango/dma.h"
#include "../libmango/i2s.h"
#include "sounds_wav.h"
#include "ringbuffer.h"
#include "timer.h"
#include <stdint.h>
#include <stdbool.h>

#define AMP_ATTENUATION 8

// Initializes all sounds by attenuating amplitudes to playable amounts.
void sounds_init();
// Plays sounds if I2S transfer completed
void sounds_play();
void play_fire();
void play_thrust();
void play_beat1();
void play_beat2();
void play_bangSmall();
void play_bangMedium();
void play_bangLarge();
void play_saucerSmall();
void play_saucerBig();
void play_extraShip();
