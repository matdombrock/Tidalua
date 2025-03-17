#ifndef UTIL_H
#define UTIL_H
#include <cstdarg>
#include <cstdio>
#include "globals.h"
void debug(const char *format, ...) {
    if (_sys.output_mode == 1) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

#endif // UTIL_H
