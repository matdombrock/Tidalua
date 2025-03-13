# wavescript
An experimental synthesizer that uses Lua scripts to generate sound.


# Dependencies
```
portaudio
portaudio-devel
```

# Building
```bash
./build.sh
```

The binary and scripts will be placed in the `./out` directory.

# Running
```bash
./wavescript <script_path>.lua [ v | d ]
```
Example:
```bash
./wavescript ./scripts/example1.lua
```

- `v` - Visualizer mode (very experimental and slow)
- `d` - Debug mode

# Scripting
- Scripts are run periodically as the synth is running.
- Scripts will "hot reload" as the synth is running. Providing a "live coding" environement.
- Scripts are run in the context of the synth. 
    - This means that you can access the synth's state and functions.
- Use the `sample_num` global to get the current sample number. 
    - This is the basis for creating time based effect.

## Lua API
### `pitch(val, oscillator)`
- `val` - The pitch value to set (factor of 220Hz)
- `oscillator` - The oscillator number to set the pitch for (0 or 1)

### `amp(val, oscillator)`
- `val` - The amplitude value to set (0.0 - 1.0)
- `oscillator` - The oscillator number to set the amplitude for (0 or 1)

### `waveform(val, oscillator)`
- `val` - The waveform value to set (0 - 3)
    - 0 - Off
    - 1 - Sine
    - 2 - Square
    - 3 - Sawtooth
- `oscillator` - The oscillator number to set the waveform for (0 or 1)

### Examples
See the `scripts` directory for examples.
