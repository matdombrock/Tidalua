#include <math.h>
#include "vis.h"
#include "globals.h"

typedef struct {
  float phase1;
  float phase2;
} Synth_Internal;


const float phase_increment = 2.0f * M_PI * FREQUENCY / ((float)SAMPLE_RATE / DOWNSAMPLE);

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


void synth_get_buffer(Synth_Internal *data, float *out) {
  for (int i = 0; i < BUFFER_SIZE; i++) {
        float sample1 = AMPLITUDE * synth_get_sample(data->phase1, 0);
        float sample2 = AMPLITUDE * synth_get_sample(data->phase2, 1);
        float mix = ((sample1 * _synth[0].amp) + (sample2 * _synth[1].amp)) / 2.0f;
        *out++ = mix;

        data->phase1 += phase_increment * _synth[0].pitch;
        if (data->phase1 >= 2.0f * M_PI) {
            data->phase1 -= 2.0f * M_PI;
        }
        data->phase2 += phase_increment * _synth[1].pitch;
        if (data->phase2 >= 2.0f * M_PI) {
            data->phase2 -= 2.0f * M_PI;
        }
        //
        vis_collect_sample(i, mix);
        vis_render();
        _sys.sample_num++;
    }
}


