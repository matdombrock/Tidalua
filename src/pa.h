#include <stdio.h>
#include <math.h>
#include <portaudio.h>

#include "globals.h"
#include "util.h"
#include "synth.h"
#include "luaBinds.h"


static int synth_callback(const void *input_buffer, void *output_buffer,
        unsigned long buffer_size,
        const PaStreamCallbackTimeInfo *time_info,
        PaStreamCallbackFlags status_flags,
        void *user_data)
{
    Synth_Internal *data = (Synth_Internal *)user_data;
    float *out = (float *)output_buffer;
    luaB_run(); 
    // Generate samples
    synth_get_buffer(data, out); 
    return paContinue;
}

int pa_init() {
  PaError err;
  PaStream *stream;
  Synth_Internal data;

  data.phase1 = 0.0f;
  data.phase2 = 0.0f;

  err = Pa_Initialize();
  if (err != paNoError) {
    fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
    return 1;
  }
  err = Pa_OpenDefaultStream(&stream,
      0,          // no input channels
      1,          // mono output
      paFloat32,  // 32-bit floating point output
      (float)SAMPLE_RATE / DOWNSAMPLE,
      BUFFER_SIZE,
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
  printf("Console output mode: %s\n", modes[_sys.output_mode]);
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
