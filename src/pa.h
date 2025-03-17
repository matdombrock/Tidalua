#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#include "globals.h"
#include "util.h"
#include "vis.h"
#include "synth.h"
#include "luaBinds.h"


typedef struct {
  float phase1;
  float phase2;
  float phase_increment;
} Synth_Internal;

typedef struct {
  int freq;
  int amp;
  int sample_rate;
  int frames_per_buffer;
  int output_mode;
  float downsample;
} Pa_Opts;

static int synth_callback(const void *input_buffer, void *output_buffer,
        unsigned long frames_per_buffer,
        const PaStreamCallbackTimeInfo *time_info,
        PaStreamCallbackFlags status_flags,
        void *user_data)
{
    Synth_Internal *data = (Synth_Internal *)user_data;
    float *out = (float *)output_buffer;
    unsigned long i;

    luaB_run(); 

    // Generate samples
    for (i = 0; i < frames_per_buffer; i++) {
        float sample1 = AMPLITUDE * synth_get_sample(data->phase1, 0);
        float sample2 = AMPLITUDE * synth_get_sample(data->phase2, 1);
        float mix = ((sample1 * _synth[0].amp) + (sample2 * _synth[1].amp)) / 2.0f;
        *out++ = mix;

        data->phase1 += data->phase_increment * _synth[0].pitch;
        if (data->phase1 >= 2.0f * M_PI) {
            data->phase1 -= 2.0f * M_PI;
        }
        data->phase2 += data->phase_increment * _synth[1].pitch;
        if (data->phase2 >= 2.0f * M_PI) {
            data->phase2 -= 2.0f * M_PI;
        }
        //
        vis_collect_sample(i, mix);
        vis_render();
        _sys.sample_num++;
    } 
    return paContinue;
}

inline int pa_init(Pa_Opts opts, PaStreamCallback synth_callback) {
  PaError err;
  PaStream *stream;
  Synth_Internal data;

  data.phase1 = 0.0f;
  data.phase2 = 0.0f;
  data.phase_increment = 2.0f * M_PI * opts.freq / ((float)opts.sample_rate / opts.downsample);

  err = Pa_Initialize();
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    return 1;
  }
  err = Pa_OpenDefaultStream(&stream,
      0,          // no input channels
      1,          // mono output
      paFloat32,  // 32-bit floating point output
      (float)opts.sample_rate / opts.downsample,
      opts.frames_per_buffer,
      synth_callback,
      &data);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    Pa_Terminate();
    return 1;
  }

  err = Pa_StartStream(stream);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 1;
  }
  printf("------- AUDIO SYSTEM STARTED -------\n");
  printf("Press ENTER to stop the audio...\n");
  char *modes[] = {"none", "debug", "visualizer"};
  printf("Console output mode: %s\n", modes[opts.output_mode]);
  getchar();

  err = Pa_StopStream(stream);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
  }

  err = Pa_CloseStream(stream);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
  }

  Pa_Terminate();
  return 0;
}
