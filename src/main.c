#include <stdio.h>
#include <math.h>
#include <portaudio.h>
#include <string.h>
#include "../lua/src/lua.h"
#include "../lua/src/lauxlib.h"
#include "../lua/src/lualib.h"

#include "config.h"

#include "globals.h"



//







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

    
    return 0;
}
