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
            .env = {0.1f, 0.1f, 0.1f},
            .env_pos = 9999.0f,
            .env_enabled = 0,
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

float synth_get_env(int osc) {
    float env = 0.0f;
    float pos = _synth[osc].env_pos;
    float attack_t = _synth[osc].env[0];
    float sustain_t = _synth[osc].env[1];
    float release_t = _synth[osc].env[2];
    
    if (pos < attack_t) { // Attack
        env = pos / attack_t;
    } 
    else if (pos < (attack_t + sustain_t)) { // Sustain
        env = 1.0f;
    } 
    else { // Release
        env = 1.0f - (pos - attack_t - sustain_t) / release_t;
    }

    _synth[osc].env_pos += (1.0f / SAMPLE_RATE) * _sys.speed;
    if (_synth[osc].env_pos > (attack_t + release_t + sustain_t)) {
        _synth[osc].env_pos = attack_t + release_t + sustain_t;
    }
    return env;
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
            sample = 2.0f * fabs(2.0f * (phase / (2.0f * M_PI)) - 1.0f) - 1.0f;
            break;
        case 5: // Noise
            sample = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
            break;
        default: // Off
            sample = 0;
            break;
    }
    if (_synth[osc].env_enabled) sample *= synth_get_env(osc);
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
// TODO: Can these be static?
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
                _vis.rms_buffer[ii][_vis.rms_index] = 0;
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
            sample *= _synth[ii].amp;
            samples[ii] = sample;
            _vis.rms_buffer[ii][_vis.rms_index] = sample;
        }
        float mixL = 0;
        float mixR = 0;
        for (int ii = 0; ii < OSC_COUNT; ii++) {
            // Negative values pan left, positive values pan right
            float pan = _synth[ii].pan;
            float panL = 0.5f - 0.5f * pan;
            float panR = 0.5f + 0.5f * pan;
            mixL += panL * samples[ii];
            mixR += panR * samples[ii];
        }
        /*mixL = mixL / (float)enabled_count;*/
        /*mixR = mixR / (float)enabled_count;*/
        // Bus lowpass filter
        mixL = synth_lowpass(0, mixL, _bus.lp_cutoff, _bus.lp_resonance / SAMPLE_RATE);
        mixR = synth_lowpass(1, mixR, _bus.lp_cutoff, _bus.lp_resonance / SAMPLE_RATE);
        // Bus AMPLITUDE
        mixL *= _bus.amp;
        mixR *= _bus.amp;
        // Add to bus rms_buffer
        _vis.rms_buffer_bus[0][_vis.rms_index] = mixL;
        _vis.rms_buffer_bus[1][_vis.rms_index] = mixR;
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
        _vis.rms_index++;
        _sys.sample_num++;
    }
    // Calculate RMS
    if (_vis.rms_index >= RMS_WINDOW) {
        for (int i = 0; i < OSC_COUNT; i++) {
            float sum = 0.0f;
            for (int j = 0; j < RMS_WINDOW; j++) {
                sum += _vis.rms_buffer[i][j] * _vis.rms_buffer[i][j];
            }
            _vis.rms[i] = sqrtf(sum / RMS_WINDOW);
        }
        // Calculate bus RMS
        float sumL = 0.0f;
        float sumR = 0.0f;
        for (int i = 0; i < RMS_WINDOW; i++) {
            sumL += _vis.rms_buffer_bus[0][i] * _vis.rms_buffer_bus[0][i];
            sumR += _vis.rms_buffer_bus[1][i] * _vis.rms_buffer_bus[1][i];
        }
        _vis.rms_bus[0] = sqrtf(sumL / RMS_WINDOW);
        _vis.rms_bus[1] = sqrtf(sumR / RMS_WINDOW);
        _vis.rms_index = 0;
        // _vis.rms_bus_history uses a FILO pattern
        for (int i = 0; i < (VIS_RMS_BUS_HIST - 1); i++) {
            _vis.rms_bus_history[0][i] = _vis.rms_bus_history[0][i + 1];
            _vis.rms_bus_history[1][i] = _vis.rms_bus_history[1][i + 1];
        }
        _vis.rms_bus_history[0][VIS_RMS_BUS_HIST - 1] = _vis.rms_bus[0];
        _vis.rms_bus_history[1][VIS_RMS_BUS_HIST - 1] = _vis.rms_bus[1];
    }
}

void synth_lua() {
    luaB_run();
}
