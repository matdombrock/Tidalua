#pragma once
#include "config.h"

// System state
typedef struct {
    char *filepath;
    int sample_num;
    int tick_num;
    int output_mode;
    float sample_acc[VIS_BUF_SIZE];
    int sample_acc_i;
    float tick_speed;
} System;

System _sys = {
    .filepath = "empty",
    .sample_num = 0,
    .tick_num = 0,
    .output_mode = 0,
    .sample_acc = {0},
    .sample_acc_i = 0,
    .tick_speed = 1.0f,
};

// Synth state
typedef struct {
    float freq;
    float detune;
    float amp;
    float pan;
    float ar[2];
    float ar_pos;
    int ar_enabled;
    float lp_cutoff;
    float lp_resonance;
    int lp_enabled;
    float hp_cutoff;
    float hp_resonance;
    int hp_enabled;
    int wave;
    int enabled;
} Synth;

Synth _synth[OSC_COUNT];

typedef struct {
    float lp_cutoff;
    float lp_resonance;
} Bus;
Bus _bus = {
    .lp_cutoff = 20000.0f,
    .lp_resonance = 1.0f,
};
