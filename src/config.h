#define SAMPLE_RATE 44100
#define DOWNSAMPLE 1 // Downsample factor (1 = no downsample)
#define FREQUENCY 440.0 / 2 // Base frequency (A4)
#define AMPLITUDE 0.5 // Amplitude of the wave
#define BUFFER_SIZE 256 // Number of frames (samples) per buffer
#define VIS_BUF_SIZE 256 // Size of the visualizer buffer
#define VIS_DOWNSAMPLE 4  // Downsample factor (1 = no downsample)
#define VIS_STRETCH 0 // Stretch the visualizer when downsampled
#define VIS_WIDTH 128 // Width of the terminal visualizer
#define LUA_RATE 8 // Lua code will run every N audio buffer fills
#define OSC_COUNT 8
