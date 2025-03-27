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
    float luatime;
    float luatimes[LUA_TIME_WINDOW];
    int luatimes_index;
    char keypress;
    float memory[16];
} System;

System _sys = {
    .filepath = "empty",
    .sample_num = 0,
    .tick_num = 0,
    .seconds = 0.0f,
    .output_mode = 0,
    .speed = 1.0f,
    .luatime = 0.0f,
    .luatimes = {0.0f},
    .luatimes_index = 0,
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
    float amp;
    float lp_cutoff;
    float lp_resonance;
} Bus;
Bus _bus = {
    .amp = 1.0f,
    .lp_cutoff = 20000.0f,
    .lp_resonance = 1.0f,
};

typedef struct {
    float rms_buffer[OSC_COUNT][RMS_WINDOW];
    float rms[OSC_COUNT];
    float rms_buffer_bus[2][RMS_WINDOW];
    float rms_bus[2];
    int rms_index;
    float rms_bus_history[2][VIS_RMS_BUS_HIST];
} Vis;
Vis _vis = {
    .rms_buffer = {{0.0f}},
    .rms = {0.0f},
    .rms_buffer_bus = {{0.0f}},
    .rms_bus = {0.0f},
    .rms_index = 0,
    .rms_bus_history = {{0.0f}},
};
