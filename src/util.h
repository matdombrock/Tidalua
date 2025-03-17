#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include <math.h>
#include <portaudio.h>
#include <string.h>
// There is obviously some bullshit going on here
// These lua includes should not be needed
#include "../lua/src/lua.h"
#include "../lua/src/lauxlib.h"
#include "../lua/src/lualib.h"
#include "globals.h"

void debug(const char *format, ...) {
    if (_sys.output_mode != 1) return;
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

#endif // UTIL_H
