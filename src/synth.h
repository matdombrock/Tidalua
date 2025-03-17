#pragma once
#include <math.h>
#include <time.h>
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
            .pan = 0.0f,
            .wave = 1,
            .enabled = 0,
        };
        _synth[i] = def; 
    }
    // Activate the first oscillator
    _synth[0].enabled = 1;
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

// Implements an Infinite Impulse Response (IIR) lowpass filter
float synth_lowpass(float input, float cutoff, float dt) {
    static float prev_input = 0.0f;
    static float prev_output = 0.0f;

    float RC = 1.0f / (2.0f * M_PI * cutoff);
    float alpha = dt / (RC + dt);

    float current_output = alpha * input + (1.0f - alpha) * prev_output;

    prev_input = input;
    prev_output = current_output;

    return current_output;
}


void synth_get_buffer(Synth_Internal *data, float *out) {
  for (int i = 0; i < BUFFER_SIZE; i++) {
        float samples[OSC_COUNT];
        int enabled_count = 0;
        for (int ii = 0; ii < OSC_COUNT; ii++) {
            if (!_synth[ii].enabled) {
                samples[ii] = 0;
                continue;
            }
            enabled_count++;
            samples[ii] = AMPLITUDE * synth_get_sample(data->phase[ii], 0);
        }
        float mixL = 0;
        float mixR = 0;
        for (int ii = 0; ii < OSC_COUNT; ii++) {
            // Negative values pan left, positive values pan right
            float pan = _synth[ii].pan;
            float panL = 0.5f - 0.5f * pan;
            float panR = 0.5f + 0.5f * pan;
            mixL += panL * _synth[ii].amp * samples[ii];
            mixR += panR * _synth[ii].amp * samples[ii];
        }
        mixL = mixL / (float)enabled_count;
        mixR = mixR / (float)enabled_count;
        mixL = synth_lowpass(mixL, _bus.lp_cutoff, _bus.lp_resonance / SAMPLE_RATE);
        mixR = synth_lowpass(mixR, _bus.lp_cutoff, _bus.lp_resonance / SAMPLE_RATE);
        /**out++ = mix;*/
        out[i * 2] = mixL; // Left
        out[i * 2 + 1] = mixR; // Right

        // Increment phase
        for (int ii = 0; ii < OSC_COUNT; ii++) {
            data->phase[ii] += synth_phase_increment(_synth[ii].freq, _synth[ii].detune);
            if (data->phase[ii] >= 2.0f * M_PI) {
                data->phase[ii] -= 2.0f * M_PI;
            }
        }
        //
        vis_collect_sample(i, mixL);
        vis_render();
        _sys.sample_num++;
    }
}

void synth_lua() {
    debug("\nlua start\n");
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        perror("clock_gettime");
        return;
    }
    luaB_run();
    struct timespec ts2;
    if (clock_gettime(CLOCK_REALTIME, &ts2) == -1) {
        perror("clock_gettime");
        return;
    }
    // Calculate the time difference in nano seconds
    long diff = ts2.tv_nsec - ts.tv_nsec;
    // Log in microseconds
    debug("lua time: %dµs \n", diff / 1000);
}
