-- example6.lua: Rhythmic Beat Generator
-- This example creates a dynamic beat pattern using amplitude modulation
-- and waveform switching to create interesting percussive sounds

-- Variables to control the rhythm pattern
local tempo = 480 -- beats per minute (much faster)
local beat_duration = (60 / tempo) * 44100 -- convert to samples (removed multiplier)
local kick_pattern = {1, 0, 0, 0, 1, 0, 0, 0} -- Basic four-on-the-floor pattern
local hat_pattern = {0, 0, 1, 0, 0, 0, 1, 0} -- Offbeat hi-hat pattern
local current_step = math.floor((sample_num / beat_duration) % 8) + 1

-- Create a kick drum using oscillator 0
if kick_pattern[current_step] == 1 then
    -- Get the position within the current beat step (0.0 - 1.0)
    local position = (sample_num % beat_duration) / beat_duration
    -- Frequency envelope: start high and quickly drop for kick drum sound
    local pitch_env = 4.0 * math.max(0, 1.0 - position * 20.0)
    pitch(pitch_env, 0)
    -- Amplitude envelope: quicker attack, faster decay
    local amp_env = math.max(0, 1.0 - position * 6.0)
    amp(amp_env, 0)
    -- Use a sine wave for the kick
    wave(1, 0)
else
    -- Silence oscillator 0 when not triggering
    amp(0, 0)
end

-- Create a hi-hat using oscillator 1
if hat_pattern[current_step] == 1 then
    -- Get the position within the current beat step (0.0 - 1.0)
    local position = (sample_num % beat_duration) / beat_duration
    -- Use a high frequency for hi-hat sound
    pitch(8.0, 1)
    -- Quick decay envelope for hihat (even faster decay)
    local amp_env = math.max(0, 0.6 - position * 16.0)
    amp(amp_env, 1)
    -- Use noise-like square wave for hi-hat
    wave(2, 1)
else
    -- Add more frequent ghost notes with low volume
    local mini_step = math.floor((sample_num / (beat_duration/2)) % 8)
    if mini_step == 1 or mini_step == 3 or mini_step == 5 or mini_step == 7 then
        local position = (sample_num % (beat_duration/2)) / (beat_duration/2)
        local amp_env = math.max(0, 0.3 - position * 24.0)
        amp(amp_env, 1)
        pitch(10.0, 1)
        wave(2, 1)
    else
        -- Silence oscillator 1 when not triggering
        amp(0, 1)
    end
end

-- Add visual feedback to show which step we're on
if sample_num % 1000 == 0 then
    local beat_display = ""
    for i=1, 8 do
        if i == current_step then
            if kick_pattern[i] == 1 then
                beat_display = beat_display .. "K"
            elseif hat_pattern[i] == 1 then
                beat_display = beat_display .. "H"
            else
                beat_display = beat_display .. "+"
            end
        else
            if kick_pattern[i] == 1 and hat_pattern[i] == 1 then
                beat_display = beat_display .. "B"
            elseif kick_pattern[i] == 1 then
                beat_display = beat_display .. "k"
            elseif hat_pattern[i] == 1 then
                beat_display = beat_display .. "h"
            else
                beat_display = beat_display .. "-"
            end
        end
    end
    print("Current pattern: " .. beat_display)
end
