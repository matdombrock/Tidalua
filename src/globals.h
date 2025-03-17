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

extern System _sys = {
    .filepath = "empty",
    .sample_num = 0,
    .output_mode = 0,
    .sample_acc = {0},
    .sample_acc_i = 0,
};

// Synth state
typedef struct {
    float pitch;
    float amp;
    int wave;
} Synth;

extern Synth _synth[] = {
    {
        .pitch = 1.0f,
        .amp = 1.0f,
        .wave = 1,
    },
    {
        .pitch = 1.0f,
        .amp = 1.0f,
        .wave = 0,
    },
};

#endif // GLOBALS_H
