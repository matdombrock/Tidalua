# 🌊 Wavescript 🌊
An experimental synthesizer that uses Lua scripts to generate sound in a "live coding" environment.

What is it?
- Modular synthesizer
- Tracker
- Live coding environment

### Scripting Example
```lua

```

# OS Support
- ✅ Linux
- ✅ MacOS
- ❓ Windows 

# Building
Initialise the build environment. This script downloads and sets up the necessary dependencies and builds the project.
```bash
./init.sh
```

Once the project has been initalised you can rebuild it with:
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

## Lua API Globals

### `seconds`
The number of seconds that have passed since the synth started.

### `tick`
The current tick number. This is the basis for creating time based effects.

The tick speed can be set with the `tick_speed` function.

The Lua script is run once per tick.

## Lua API Functions

### `dbg(message)`
Prints a message to the console if the console output mode is set to debug.

### `enable(oscillator_number)`
Enables an oscillator. The first oscillator is enabled by default.

### `disable(oscillator_number)`
Disables an oscillator.

### `freq(frequency, oscillator_number)`
Sets the frequency of an oscillator in Hz.

### `note(note, oscillator_number)`
Sets the frequency of an oscillator to a given note name.

### `amp(amplitude, oscillator_number)`
Sets the amplitude of an oscillator.

### `wave(waveform_number, oscillator_number)`
Sets the waveform of an oscillator.
```
0 = Off
1 = Sine
2 = Square
3 = Sawtooth
4 = Triangle
5 = Noise
```

### `pan(pan, oscillator_number)`
Sets the pan of an oscillator.

A value of -1 is full left, 0 is center, and 1 is full right.

### `atk_rel(attack, release, oscillator_number)`
Sets the attack and release of an oscillator.

Measured in ticks.

### `solo(oscillator_number)`
Sets an oscillator to solo mode. Only the soloed oscillator will be heard.

### `lowpass(cutoff, resonance, oscillator_number)`
Sets the lowpass filter of an oscillator.

### `highpass(cutoff, resonance, oscillator_number)`
Sets the highpass filter of an oscillator.

### `tick_speed(speed)`
Sets the speed of the script in ticks per second.

### `bus_lowpass(cutoff, resonance)`
Sets the lowpass filter of the master bus.

### Examples
See the `scripts` directory for examples.

