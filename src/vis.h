#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "globals.h"
#include "util.h"
#define VIS_WIDTH 24
#define VIS_HEIGHT 16
void vis_clear() {
    printf("\033[2J");
    // Clear the console
    system("clear");
}

void vis_render(int vtick) {
    for (int y = 0; y < VIS_HEIGHT; y++) {
        for (int x = 0; x < VIS_WIDTH; x++) {
            int pos = vtick % (VIS_WIDTH * VIS_HEIGHT);
            if (pos == (y * VIS_WIDTH + x)) {
                printc(COLOR_RED, "▓▓");
            } else {
                printc(COLOR_YELLOW, "░░");
            }
        }         
        printf("\n");
    } 
}

// Configure terminal for non-blocking input
void vis_nonblocking() {
    // This causes wierd issues on macOS
    // Isues with colors and wide characters
    // Check if we are on apple
#ifdef __APPLE__
    return;
#endif
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
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    oldt.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    // Restore blocking mode for stdin
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}

int vis_input() {
    // Handle non-blocking input
    int ch = getchar();
    if (ch != -1) {
        _sys.keypress = ch;
    }
    if (ch == EOF) {
        return 0; // Stop running
    }
    return 1; // Keey running 
}

void vis_loop() {
    int run = 1;
    int vtick = 0;
    // Initialize non-blocking input
    vis_nonblocking();
    while (run) {
        vis_clear();
        printf("🌘🌊 Tidalua 🌊🌒\n");
        vis_render(vtick);
        /*run = vis_input();*/
        printf("🌊 %.2f | %d | %d | %c\n", _sys.speed, _sys.tick_num, vtick, _sys.keypress);
        printf("🌘 %s\n", _sys.filepath);
        vtick++;
        usleep(22 * 1000); // Sleep for 100 milliseconds to reduce CPU usage
    }
    // Restore terminal settings
    vis_blocking();
}
