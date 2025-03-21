#pragma once
#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#include "config.h"
#include "util.h"
#include "synth.h"
#include "vis.h"


static int pa_callback(const void *input_buffer, void *output_buffer,
        unsigned long buffer_size,
        const PaStreamCallbackTimeInfo *time_info,
        PaStreamCallbackFlags status_flags,
        void *user_data)
{
    Synth_Internal *data = (Synth_Internal *)user_data;
    float *out = (float *)output_buffer;
    // Run Lua
    synth_lua(); 
    // Generate samples
    synth_get_buffer(data, out); 
    return paContinue;
}

int pa_init() {
  PaError err;
  PaStream *stream;
  Synth_Internal data;
  for (int i = 0; i < OSC_COUNT; i++) {
    data.phase[i] = 0.0f;
  }
  debug("pa: init\n");
  err = Pa_Initialize();
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    return 1;
  }
  debug("pa: open default stream\n");
  err = Pa_OpenDefaultStream(&stream,
      0,          // input channels
      2,          // output channels
      paFloat32,  // 32-bit floating point output
      (float)SAMPLE_RATE / DOWNSAMPLE,
      BUFFER_SIZE,
      pa_callback,
      &data);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    Pa_Terminate();
    return 1;
  }

  debug("pa: start stream\n");
  err = Pa_StartStream(stream);
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    Pa_CloseStream(stream);
    Pa_Terminate();
    return 1;
  }
  printf("------- AUDIO SYSTEM STARTED -------\n");
  char modes[][32] = {"none", "debug", "visualizer"};
  printf("Console output mode: %s\n", modes[_sys.output_mode]);
  // Clear the Console
  system("clear");

  if (_sys.output_mode == 2) {
    // Start the visualizer loop
    vis_loop();
  }
  
  printf("Press ENTER to stop the audio...\n");
  // Wait for ENTER
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
