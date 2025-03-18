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

## Lua API

### Examples
See the `scripts` directory for examples.

