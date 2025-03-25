#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include "globals.h"
#include "util.h"
#include "notes.h"
#define VIS_WIDTH 24
#define VIS_HEIGHT 16
#define VIS_NULL_CHAR L'/'
#define VIS_ENABLE_INPUT 0


// This causes wierd issues on macOS
// Isues with colors and wide characters
// Check if we are on apple
#ifdef __APPLE__
#define VIS_ENABLE_INPUT 0
#endif

void vis_clear() {
    printf("\033[2J");
    // Clear the console
    system("clear");
}

wchar_t vis_frame[VIS_HEIGHT][VIS_WIDTH * 2] = {L'▓'};
int vis_frame_color[VIS_HEIGHT][VIS_WIDTH] = {COLOR_BLACK};

void vis_frame_set(int x, int y, wchar_t c[2], int color) {
    vis_frame[y][x * 2] = c[0];
    vis_frame[y][x * 2 + 1] = c[1];
    vis_frame_color[y][x] = color;
}

wchar_t vis_frame_get(int x, int y, int offset) {
    return vis_frame[y][x * 2 + offset];
}

void vis_prerender(int vtick) {
    for (int y = 0; y < VIS_HEIGHT; y++) {
        for (int x = 0; x < VIS_WIDTH; x++) {
            if (y == 0 || y == VIS_HEIGHT - 1 || x == 0 || x == VIS_WIDTH - 1) {
                wchar_t px[2] = {L'▒', L'░'};
                vis_frame_set(x, y, px, COLOR_GREEN); 
            } else {
                wchar_t px[2] = {L' ', L' '};
                vis_frame_set(x, y, px, COLOR_GREEN);
            }         
        }
    }
    wchar_t corner[2] = {L'▓', L'▓'};
    vis_frame_set(0, 0, corner, COLOR_RED);
    vis_frame_set(VIS_WIDTH - 1, 0, corner, COLOR_RED);
    vis_frame_set(0, VIS_HEIGHT - 1, corner, COLOR_RED);
    vis_frame_set(VIS_WIDTH - 1, VIS_HEIGHT - 1, corner, COLOR_RED);

    //
    /*wchar_t title[2] = {L'O', L'S'};*/
    /*vis_frame_set(1, 1, title, COLOR_NONE);*/

    for (int i = 0; i < OSC_COUNT; i++) {
        // Enabled
        if (_synth[i].enabled) {
            wchar_t px[2] = {L' ', L'▶'};
            vis_frame_set(1, i + 2, px, COLOR_GREEN);
        } else {
            // ◀
            wchar_t px[2] = {L' ', L'▶'};
            vis_frame_set(1, i + 2, px, COLOR_RED);
        }
        // Note
        char *note = note_by_freq(_synth[i].freq).name;
        int is_sharp = _synth[i].freq > note_by_name(note).freq;
        int is_flat = _synth[i].freq < note_by_name(note).freq;
        int color = COLOR_GREEN;
        if (is_sharp) {
            color = COLOR_YELLOW;
        }
        if (is_flat) {
            color = COLOR_BLUE;
        }
        wchar_t note_a[2] = {note[0], note[1]};
        vis_frame_set(3, i + 2, note_a, color);
        wchar_t note_b[2] = {(note[2] != '\0' ? note[2] : L' '), (note[3] != '\0' ? note[3] : L' ')};
        vis_frame_set(4, i + 2, note_b, color);
        
        // Wave
        // Off, Sine, Square, Saw, Triangle, Noise
        // X, ∿, ⊓, ⭍, ⧊, ⚂
        wchar_t wave_symb[6] = {L'X', L'∿', L'⊓', L'⭍', L'⧊', L'⚂'};
        int wave = _synth[i].wave;
        wchar_t wavec = (wchar_t)(wave + L'0');
        wchar_t wave_char[2] = {wavec, wave_symb[wave]};
        vis_frame_set(6, i + 2, wave_char, COLOR_CYAN);

        // Volume monitor
        float rms = _vis.rms[i];
        // Scale the rms value logarithmically
        rms = logf(rms + 1) / logf(2);
        // Normalize the volume to 8 levels
        int vol = (int)(rms * 8);
        vol = vol < 8 ? vol : 8;
        vol = vol > 0 ? vol : 0;
        int vcolor = COLOR_GREEN;
        wchar_t vstr[8] = {L'='};
        for (int v = 0; v < 8; v++) {
            if (v < vol) {
                vstr[v] = L'=';
            } else {
                vstr[v] = L' ';
            }
        }
        for (int v = 0; v < 4; v++) {
            int pos = v * 2;
            wchar_t px[2] = {vstr[pos], vstr[pos + 1]};
            if (v > 1) {
                vcolor = COLOR_YELLOW;
            }
            if (v > 2) {
                vcolor = COLOR_RED;
            }
            vis_frame_set(8 + v, 2 + i, px, vcolor);
        }
    }

    int pos = (vtick / 32) % (VIS_WIDTH - 2);
    for (int i = 0; i < OSC_COUNT; i++) {
        if (_synth[i].enabled) {
            wchar_t px[2] = {L'▓', L'▓'};
            vis_frame_set(pos + 1, 0, px, COLOR_YELLOW);
        }
    }
    
}

void vis_render(int vtick) {
    for (int y = 0; y < VIS_HEIGHT; y++) {
        for (int x = 0; x < VIS_WIDTH; x++) {
            int color = vis_frame_color[y][x];
            if (color != 0) printf("\033[0;%dm", color);
            // Convert the 2 wide characters to a string
            wchar_t c0 = vis_frame_get(x, y, 0);
            wchar_t c1 = vis_frame_get(x, y, 1);
            size_t len = 1;
            if (c1 != VIS_NULL_CHAR ) {
                len = 2;
            }
            wchar_t str[len];
            str[0] = vis_frame_get(x, y, 0);
            if (len == 2) {
                str[1] = vis_frame_get(x, y, 1);
            }
            str[len] = '\0';
            printf("%ls", str);
            printf("\033[0m");
        }         
        printf("\n");
    } 
}

// Configure terminal for non-blocking input
void vis_nonblocking() { 
    if (VIS_ENABLE_INPUT == 0) {
        return;
    }
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Set non-blocking mode for stdin
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}
// Restore terminal settings to blocking mode
void vis_blocking() {
    if (VIS_ENABLE_INPUT == 0) {
        return;
    }
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    oldt.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    // Restore blocking mode for stdin
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

// Handle non-blocking input
int vis_input() {
    if (VIS_ENABLE_INPUT == 0) {
        return 1;
    } 
    int ch = getchar();
    if (ch != -1) {
        _sys.keypress = ch;
    }
    if (ch == EOF) {
        return 0; // Stop running
    }
    return 1; // Keey running 
}

void vis_init() {
    // Set the locale to support Unicode
    setlocale(LC_ALL, "");
    // Reopen stdout in wide mode
    freopen(NULL, "w", stdout);
}

void vis_loop() {
    int run = 1;
    int vtick = 0;
    // Initialize non-blocking input
    vis_nonblocking();
    while (run) {
        vis_init();
        vis_clear();
        printf("🌘🌊 Tidalua 🌊🌒\n");
        vis_prerender(vtick);
        vis_render(vtick);
        run = vis_input();
        printf("🌊 %.2f | %d | %d | %c\n", _sys.speed, _sys.tick_num, vtick, _sys.keypress);
        printf("🌘 %s\n", _sys.filepath);
        vtick++;
        usleep(22 * 1000); // Sleep to reduce CPU usage
    }
    // Restore terminal settings
    vis_blocking();
}

