-- melody_example.lua: Simple Melody Generator
-- This example creates a catchy melody with bass accompaniment

-- Musical parameters
local tempo = 240 -- BPM
local beat_duration = (60 / tempo) * 44100

-- Define a scale (C major: C, D, E, F, G, A, B)
local scale = {1.0, 1.125, 1.25, 1.333, 1.5, 1.667, 1.875, 2.0}

-- Define a melody as a sequence of notes (scale degrees, 0 = rest)
local melody = {
    {note=1, duration=1}, {note=3, duration=1}, {note=5, duration=1}, {note=8, duration=1},
    {note=5, duration=2}, {note=3, duration=1}, {note=5, duration=1}, 
    {note=1, duration=1}, {note=3, duration=1}, {note=5, duration=2},
    {note=3, duration=2}, {note=1, duration=2}
}

-- Define a bassline
local bassline = {
    {note=1, duration=4}, 
    {note=5, duration=4}, 
    {note=6, duration=4},
    {note=5, duration=4}
}

-- Calculate current position in the melody
local melody_total_beats = 0
for _, note in ipairs(melody) do
    melody_total_beats = melody_total_beats + note.duration
end

local bass_total_beats = 0
for _, note in ipairs(bassline) do
    bass_total_beats = bass_total_beats + note.duration
end

-- Calculate which note we're currently playing in melody (oscillator 0)
local current_beat_position = (sample_num / beat_duration) % melody_total_beats
local current_melody_note = 1
local beat_counter = 0

for i, note in ipairs(melody) do
    beat_counter = beat_counter + note.duration
    if current_beat_position < beat_counter then
        current_melody_note = i
        break
    end
end

-- Calculate which note we're currently playing in bassline (oscillator 1)
local current_bass_position = (sample_num / beat_duration) % bass_total_beats
local current_bass_note = 1
local bass_beat_counter = 0

for i, note in ipairs(bassline) do
    bass_beat_counter = bass_beat_counter + note.duration
    if current_bass_position < bass_beat_counter then
        current_bass_note = i
        break
    end
end

-- Calculate time position within the current note (0.0 - 1.0)
local prev_melody_beats = current_beat_position - (beat_counter - melody[current_melody_note].duration)
local note_position = prev_melody_beats / melody[current_melody_note].duration

local prev_bass_beats = current_bass_position - (bass_beat_counter - bassline[current_bass_note].duration)
local bass_note_position = prev_bass_beats / bassline[current_bass_note].duration

-- Play the melody (oscillator 0)
if melody[current_melody_note].note > 0 then
    -- Set the pitch from the scale
    pitch(scale[melody[current_melody_note].note], 0)
    
    -- Simple envelope to make it sound nice
    local env = 0.8
    -- Add a slight attack
    if note_position < 0.05 then
        env = env * (note_position / 0.05)
    end
    -- Add a release
    if note_position > 0.8 then
        env = env * (1.0 - (note_position - 0.8) / 0.2)
    end
    
    amp(env, 0)
    wave(1, 0) -- Sine wave for melody
else
    -- Rest
    amp(0, 0)
end

-- Play the bassline (oscillator 1)
if bassline[current_bass_note].note > 0 then
    -- Set the pitch from the scale (one octave down)
    pitch(scale[bassline[current_bass_note].note] / 2, 1)
    
    -- Simple envelope with slight attack and longer sustain
    local env = 0.6
    -- Add attack
    if bass_note_position < 0.1 then
        env = env * (bass_note_position / 0.1)
    end
    -- Add release
    if bass_note_position > 0.7 then
        env = env * (1.0 - (bass_note_position - 0.7) / 0.3)
    end
    
    amp(env, 1)
    wave(3, 1) -- Saw wave for bass
else
    -- Rest
    amp(0, 1)
end

-- Display current note being played
if sample_num % 2000 == 0 then
    local melody_display = ""
    for i=1, #melody do
        if i == current_melody_note then
            melody_display = melody_display .. "[" .. melody[i].note .. "]"
        else
            melody_display = melody_display .. melody[i].note
        end
        if i < #melody then
            melody_display = melody_display .. " "
        end
    end
    
    local bass_display = ""
    for i=1, #bassline do
        if i == current_bass_note then
            bass_display = bass_display .. "[" .. bassline[i].note .. "]"
        else
            bass_display = bass_display .. bassline[i].note
        end
        if i < #bassline then
            bass_display = bass_display .. " "
        end
    end
    
    print("Melody: " .. melody_display .. " | Bass: " .. bass_display)
end