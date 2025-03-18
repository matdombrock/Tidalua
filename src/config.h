#define SAMPLE_RATE 44100
#define DOWNSAMPLE 1 // Downsample factor (1 = no downsample)
#define FREQUENCY 440.0 / 2 // Base frequency (A4)
#define AMPLITUDE 0.7 // Amplitude of the wave
#define BUFFER_SIZE 128 // Number of frames (samples) per buffer
#define VIS_BUF_SIZE 256 // Size of the visualizer buffer
#define VIS_DOWNSAMPLE 4  // Downsample factor (1 = no downsample)
#define VIS_STRETCH 0 // Stretch the visualizer when downsampled
#define VIS_WIDTH 128 // Width of the terminal visualizer
#define LUA_INDEX 1 // Use lua style indexing (1 = true, 0 = false)
#define OSC_COUNT 8
