#pragma once
#include <math.h>
#include <stdlib.h>
#include "luaBinds.h"
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
            .ar = {0.1f, 0.1f, 0.1f},
            .ar_pos = 9999.0f,
            .ar_enabled = 0,
            .lp_cutoff = 20000.0f,
            .lp_resonance = 1.0f,
            .lp_enabled = 0,
            .hp_cutoff = 0.0f,
            .hp_resonance = 1.0f,
            .hp_enabled = 0,
            .wave = 1,
            .enabled = 0,
        };
        _synth[i] = def; 
    }
    // Activate the first oscillator
    _synth[0].enabled = 1;
}

float synth_get_ar(int osc) {
    float ar = 0.0f;
    float pos = _synth[osc].ar_pos;
    float attack_t = _synth[osc].ar[0];
    float sustain_t = _synth[osc].ar[1];
    float release_t = _synth[osc].ar[2];
    
    if (pos < attack_t) { // Attack
        ar = pos / attack_t;
    } 
    else if (pos < (attack_t + sustain_t)) { // Sustain
        ar = 1.0f;
    } 
    else { // Release
        ar = 1.0f - (pos - attack_t - sustain_t) / release_t;
    }

    _synth[osc].ar_pos += (1.0f / SAMPLE_RATE) * _sys.speed;
    if (_synth[osc].ar_pos > (attack_t + release_t + sustain_t)) {
        _synth[osc].ar_pos = attack_t + release_t + sustain_t;
    }
    return ar;
}
float synth_get_sample(float phase, int osc) {
    int mode = _synth[osc].wave % 6; // Wrap wave mode
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
        case 4: // Triangle
            sample = 2.0f * fabsf(2.0f * (phase / (2.0f * M_PI)) - 1.0f) - 1.0f;
            break;
        case 5: // Noise
            sample = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
            break;
        default: // Off
            sample = 0;
            break;
    }
    if (_synth[osc].ar_enabled) sample *= synth_get_ar(osc);
    return sample;
}

float synth_phase_increment(float freq, float detune) {
    return 2.0f * M_PI * freq / ((float)SAMPLE_RATE / DOWNSAMPLE) * (1.0f + detune);
}

// Implements an Infinite Impulse Response (IIR) lowpass filter
float _synth_lowpass_buf_in[2 + OSC_COUNT] = {0};
float _synth_lowpass_buf_out[2 + OSC_COUNT] = {0};
float synth_lowpass(int index, float input, float cutoff, float dt) {
    float prev_input = _synth_lowpass_buf_in[index];
    float prev_output = _synth_lowpass_buf_out[index];

    float RC = 1.0f / (2.0f * M_PI * cutoff);
    float alpha = dt / (RC + dt);

    float current_output = alpha * input + (1.0f - alpha) * prev_output;

    _synth_lowpass_buf_in[index] = input;
    _synth_lowpass_buf_out[index] = current_output;

    return current_output;
}

// Implements an Infinite Impulse Response (IIR) highpass filter
float _synth_highpass_buf_in[2 + OSC_COUNT] = {0};
float _synth_highpass_buf_out[2 + OSC_COUNT] = {0};
float synth_highpass(int index, float input, float cutoff, float dt) {
    float prev_input = _synth_highpass_buf_in[index];
    float prev_output = _synth_highpass_buf_out[index];

    float RC = 1.0f / (2.0f * M_PI * cutoff);
    float alpha = RC / (RC + dt);

    float current_output = alpha * prev_output + alpha * (input - prev_input);

    _synth_highpass_buf_in[index] = input;
    _synth_highpass_buf_out[index] = current_output;

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
            float sample = AMPLITUDE * synth_get_sample(data->phase[ii], ii);
            if (_synth[ii].lp_enabled) {
                sample = synth_lowpass(2 + ii, sample, _synth[ii].lp_cutoff, _synth[ii].lp_resonance / SAMPLE_RATE);
            }
            if (_synth[ii].hp_enabled) {
                sample = synth_highpass(2 + ii, sample, _synth[ii].hp_cutoff, _synth[ii].hp_resonance / SAMPLE_RATE);
            }
            samples[ii] = sample;
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
        // Bus lowpass filter
        mixL = synth_lowpass(0, mixL, _bus.lp_cutoff, _bus.lp_resonance / SAMPLE_RATE);
        mixR = synth_lowpass(1, mixR, _bus.lp_cutoff, _bus.lp_resonance / SAMPLE_RATE);
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
        _sys.sample_num++;
    }
}

void synth_lua() {
    luaB_run();
}
