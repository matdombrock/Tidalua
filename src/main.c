#include <string.h>
#include "pa.h"
#include "synth.h"
#include "config.h"
#include "globals.h"

int main(int argc, char *argv[])
{
    _sys.filepath = argv[1];
    if (argc > 2) {
        if (strcmp(argv[2], "d") == 0) {
            _sys.output_mode = 1;
        }
        else if (strcmp(argv[2], "v") == 0) {
            _sys.output_mode = 2;
        } 
    }   
    synth_init();
    pa_init();
    printf("exiting...\n");
    return 0;
}
