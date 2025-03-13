#include <stdio.h>
#include <math.h>
#include <portaudio.h>
#include <string.h>
#include "../lua/src/lua.h"
#include "../lua/src/lauxlib.h"
#include "../lua/src/lualib.h"

#define SAMPLE_RATE 44100
#define DOWNSAMPLE 1 // Downsample factor (1 = no downsample)
#define FREQUENCY 440.0 / 2 // Base frequency (A4)
#define AMPLITUDE 0.5 // Amplitude of the wave
#define FRAMES_PER_BUFFER 256 // Number of frames (samples) per buffer
#define VIS_BUF_SIZE 256 // Size of the visualizer buffer
#define VIS_DOWNSAMPLE 4  // Downsample factor (1 = no downsample)
#define VIS_STRETCH 0 // Stretch the visualizer when downsampled
#define VIS_WIDTH 128 // Width of the terminal visualizer
#define LUA_RATE 8 // Lua code will run every N audio buffer fills

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
    float pitch;
    float amp;
    int wave;
} Synth;

Synth _synth[] = {
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

void debug(const char *format, ...) {
    if (_sys.output_mode == 1) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}


// LUA BINDS
int luaB_debug(lua_State *L) {
    const char *msg = luaL_checkstring(L, 1);
    debug("lua: %s\n", msg);
    return 0;
}

int luaB_pitch(lua_State *L) {
    float val = luaL_checknumber(L, 1);
    int target = luaL_optinteger(L, 2, 0);
    target = target > 1 ? 1 : target;
    _synth[target].pitch = val;
    debug("lua: pitch(%f, %d)\n", val, target);
    return 0;
}
int luaB_amp(lua_State *L) {
    float val = luaL_checknumber(L, 1);
    int target = luaL_optinteger(L, 2, 0);
    target = target > 1 ? 1 : target;
    _synth[target].amp = val;
    debug("lua: amp(%f, %d)\n", val, target);
    return 0;
}
int luaB_wave(lua_State *L) {
    int val = luaL_checkinteger(L, 1);
    int target = luaL_optinteger(L, 2, 0);
    target = target > 1 ? 1 : target;
    _synth[target].wave = val;
    debug("lua: wave(%d, %d)\n", val, target);
    return 0;
}
void luaB_binds(lua_State *L) {
    lua_register(L, "dbg", luaB_debug);  // Using dbg which is shorter and not a reserved name
    lua_register(L, "pitch", luaB_pitch);
    lua_register(L, "amp", luaB_amp);
    lua_register(L, "wave", luaB_wave);
}

void luaB_run() {
    if (_sys.sample_num % LUA_RATE == 0) {
        debug("g_sys_sample_num: %d\n", _sys.sample_num);
        // Init a new lua lua_State
        lua_State *L = luaL_newstate();
        luaB_binds(L);
        luaL_openlibs(L);  // Load standard libraries
        
        // Pass system variables to Lua
        lua_pushnumber(L, _sys.sample_num);
        lua_setglobal(L, "sample_num");
        
        //luaL_dofile(L, "test2.lua");
        if (luaL_dofile(L, _sys.filepath) != LUA_OK) {
            fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1); // remove error message from stack
        }
    }
}
//

void vis_collect_sample(int index, float mix) {
    if (_sys.output_mode != 2) return;
    if (index % VIS_DOWNSAMPLE == 0) {
        _sys.sample_acc[_sys.sample_acc_i] = mix;
        _sys.sample_acc_i++;
    }
}

void vis_render() {
    if (_sys.output_mode != 2) return;
    if (_sys.sample_acc_i >= VIS_BUF_SIZE) {
        _sys.sample_acc_i = 0;
        // For each sample in the buffer
        for (int i = 0; i < VIS_BUF_SIZE; i++) {
            float norm = _sys.sample_acc[i];
            norm /= 2;
            norm += 0.5;
            int pos = floor(norm * VIS_WIDTH);
            // Should we stretch the visualizer? and repeat downsampled samples?
            int stretch = VIS_STRETCH ? VIS_DOWNSAMPLE : 1;
            // For each stretch
            for (int ii = 0; ii < stretch; ii++) {
                char line[VIS_WIDTH * 8] = ""; // Support wide characters
                // For each pixel in the visualizer
                for (int iii = 0; iii < VIS_WIDTH; iii++) {
                    if (iii == pos) {
                        /*printf(" ");*/
                        strcat(line, " ");
                    }
                    else if (iii > (VIS_WIDTH / 2)) {
                        /*printf("░");*/
                        strcat(line, "░");
                    }
                    else {
                        /*printf("▒");*/
                        strcat(line, "▒");

                    }
                }
                printf("%s", line);
                printf("| %f", _sys.sample_acc[i]);
                printf("\n");
            }
        }
    }
}



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

typedef struct {
    float phase1;
    float phase2;
    float phase_increment;
} Synth_Internal;

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

int main(int argc, char *argv[])
{
    _sys.filepath = argv[1];
    // Simple debug flag
    if (argc > 2) {
        if (strcmp(argv[2], "d") == 0) {
            _sys.output_mode = 1;
        }
        else if (strcmp(argv[2], "v") == 0) {
            _sys.output_mode = 2;
        } 
    }

    PaError err;
    PaStream *stream;
    Synth_Internal data;

    data.phase1 = 0.0f;
    data.phase2 = 0.0f;
    data.phase_increment = 2.0f * M_PI * FREQUENCY / ((float)SAMPLE_RATE / DOWNSAMPLE);

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
            FRAMES_PER_BUFFER,
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
