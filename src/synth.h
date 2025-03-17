#pragma once
#include <math.h>
#include "luaBinds.h"
#include "vis.h"
#include "globals.h"

// Represents the internal state of the oscillator
// Not directly exposed to the user
typedef struct {
  float phase[OSC_COUNT];
} Synth_Internal;

void synth_init() { 
    for (int i = 0; i < 8; i++) {
        Synth def = {
            .freq = FREQUENCY,
            .detune = 0.0f,
            .amp = 1.0f,
            .wave = 1,
        };
        _synth[i] = def; 
    }
}

float synth_get_sample(float phase, int osc) {
    int mode = _synth[osc].wave % 4; 
    float sample = 0.0f;
    switch (mode) {
        case 0: // Off
            sample = 0;
            break;
        case 1: // Sine
            sample = sinf(phase);
            break;
        case 2: // Square
            sample = sinf(phase) > 0 ? 1.0f : -1.0f;
            break;
        case 3: // Saw
            sample = 2.0f * (phase / (2.0f * M_PI)) - 1.0f;
            break;
        default: // Off
            sample = 0;
            break;
    }
    return sample;
}

float synth_phase_increment(float freq, float detune) {
    return 2.0f * M_PI * freq / ((float)SAMPLE_RATE / DOWNSAMPLE) * (1.0f + detune);
}

void synth_get_buffer(Synth_Internal *data, float *out) {
  for (int i = 0; i < BUFFER_SIZE; i++) {
        float samples[OSC_COUNT];
        for (int ii = 0; ii < OSC_COUNT; ii++) {
            samples[ii] = AMPLITUDE * synth_get_sample(data->phase[ii], 0);
        }
        float mix = 0;
        for (int ii = 0; ii < OSC_COUNT; ii++) {
            mix += _synth[ii].amp * samples[ii];
        }
        *out++ = mix;

        for (int ii = 0; ii < OSC_COUNT; ii++) {
            data->phase[ii] += synth_phase_increment(_synth[ii].freq, _synth[ii].detune);
            if (data->phase[ii] >= 2.0f * M_PI) {
                data->phase[ii] -= 2.0f * M_PI;
            }
        }
        //
        vis_collect_sample(i, mix);
        vis_render();
        _sys.sample_num++;
    }
}

void synth_lua() {
    luaB_run();
}
