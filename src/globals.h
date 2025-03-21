#pragma once
#include "config.h"

// System state
typedef struct {
    char *filepath;
    int sample_num;
    int tick_num;
    float seconds;
    int output_mode;
    int sample_acc_i;
    float speed;
    char keypress;
    float memory[16];
} System;

System _sys = {
    .filepath = "empty",
    .sample_num = 0,
    .tick_num = 0,
    .seconds = 0.0f,
    .output_mode = 0,
    .sample_acc_i = 0,
    .speed = 1.0f,
    .keypress = -1,
    .memory = {0.0f},
};

// Synth state
typedef struct {
    float freq;
    float detune;
    float amp;
    float pan;
    float env[3];
    float env_pos;
    int env_enabled;
    float lp_cutoff;
    float lp_resonance;
    int lp_enabled;
    float hp_cutoff;
    float hp_resonance;
    int hp_enabled;
    int wave;
    int enabled;
} Synth;

Synth _synth[OSC_COUNT]; // Initialized in synth.h

typedef struct {
    float lp_cutoff;
    float lp_resonance;
} Bus;
Bus _bus = {
    .lp_cutoff = 20000.0f,
    .lp_resonance = 1.0f,
};
