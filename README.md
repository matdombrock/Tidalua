# 🌊 Wavescript 🌊
An experimental synthesizer that uses Lua scripts to generate sound in a "live coding" environment.

### Scripting Example
```lua
-- Set the speed of the LFO
local speed = 2048 * 128
-- Normalize the sample number  to the range 0 -> 1
-- This is also our relative position in the LFO
local norm = (sample_num % speed) / speed
-- Create a sine wave with a period of 1
local sin = math.sin(norm * math.pi)
-- Set the pitch range to 0 -> 4 by multiplying the sine wave by 4
local pitch1 = sin * 4
-- Use the pitch1 value to control the pitch of the first oscillator
pitch(pitch1, 0)
-- Lower the volume of the first oscillator a bit
amp(0.75, 0)

-- Set the pitch2 value with the floor function to create a stepped effect for the second oscillator
-- Add 1 to offset it to the range 1 -> 5
local pitch2 = math.floor(sin * 4) + 1
pitch(pitch2, 1)

-- Rotate through all waves for the second oscillator
-- We have to get a little tricky here to keep it in sync with the pitch changes
-- There are currently only 3 waves so we have to make sure we don't go out of bounds
-- Going out of bounds is not an error but it will wrap around to 0 which is off
local wave2 = math.floor(sin * 4) + 1
if wave2 > 3 then
  wave2 = 3
end
wave(wave2, 1)

-- Tremolo effect
-- Only plays in the second half of the script
if norm > 0.5 then
  amp(0.5 + (math.sin(sample_num / 8) * 0.4), 1)
else
  amp(0.5, 1)
end
```

# OS Support
- ✅ Linux
- ✅ MacOS
- ❓ Windows 

# Dependencies
```
portaudio
portaudio-devel
```

The `portaudio-dev` library is only required to build the project on Linux.

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
- Scripts will "hot reload" as the synth is running. Providing a "live coding" environment.
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
