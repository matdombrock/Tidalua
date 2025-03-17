#pragma once
#include <stdio.h>
#include <stdarg.h>
#include "globals.h"

void debug(const char *format, ...) {
    if (_sys.output_mode != 1) return;
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
