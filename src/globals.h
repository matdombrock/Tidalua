#ifndef GLOBALS_H
#define GLOBALS_H

#include "config.h"

// System state
typedef struct {
    char *filepath;
    int sample_num;
    int output_mode;
    float sample_acc[VIS_BUF_SIZE];
    int sample_acc_i;
} System;

System _sys = {
    .filepath = "empty",
    .sample_num = 0,
    .output_mode = 0,
    .sample_acc = {0},
    .sample_acc_i = 0,
};

// Synth state
typedef struct {
    float freq;
    float detune;
    float amp;
    int wave;
} Synth;

Synth _synth[] = {
    {
        .freq = FREQUENCY,
        .detune = 0.0f,
        .amp = 1.0f,
        .wave = 1,
    },
    {
        .freq = FREQUENCY,
        .detune = 0.0f,
        .amp = 1.0f,
        .wave = 0,
    },
};

#endif // GLOBALS_H
