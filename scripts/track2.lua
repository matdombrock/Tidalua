tick_speed(0.6) -- max 1.0

local tt = tick % 128

-- Kick drum
-- A simple kick drum is made by detuning a sine wave oscillator
atk_rel(0.005, 0.15, 1)
if tt == 1 then
  amp(1.5, 1)
  note("G3", 1)
elseif tt < 32 then
  detune(-tt / 32, 1)
  amp(1.5-tt / 32, 1)
elseif tt == 32 then
  amp(0, 1)
end

-- Snare drum
-- A simple snare drum is made with a noise oscillator
enable(2)
atk_rel(0.005, 0.2, 2)
wave(5, 2)
highpass(250, 4, 2)
lowpass(7000, 3, 2)
if tt == 1 then
  amp(0, 2)
elseif tt == 64 then
  amp(1, 2)
  note("C4", 2)
elseif tt == 96 then
  amp(0, 2)
end

-- Hi-hat
-- A simple hi-hat is also made with a noise oscillator
enable(3)
atk_rel(0.005, 0.15, 3)
wave(5, 3)
highpass(10000, 4, 3)
local hh_speed = 32
if (tick % 512) > 256 then
  hh_speed = 16
end
if tt % hh_speed == 1 then
  amp(1, 3)
  note("C4", 3)
elseif tt % hh_speed == 9 then
  amp(0, 3)
end

-- Melody
enable(4)
wave(1, 4)
local lfo = math.sin(seconds / 2)
highpass(10000 + lfo * 10000, 1, 4)
if tt == 1 then
  note("C5", 4, 4)
elseif tt == 32 then
  note("D5", 4, 4)
elseif tt == 64 then
  note("E5", 4, 4)
elseif tt == 96 then
  note("F5", 4, 4)
end

-- Bass
enable(5)
wave(2, 5)
lowpass(2000, 1, 5)
atk_rel(0.005, 0.15, 5)
if tt == 1 then
  note("C2", 5, 5)
elseif tt == 32 then
  if (math.random() > 0.5) then
    note("C2", 5, 5)
  elseif (math.random() > 0.5) then
    note("D3", 5, 5)
  else
    amp(0, 5)
  end
elseif tt == 64 then
  amp(1, 5)
  note("E2", 5, 5)
elseif tt == 96 then
  note("F2", 5, 5)
end

-- Chords
enable(6)
enable(7)
enable(8)
wave(1, 6)
wave(1, 7)
wave(1, 8)
local speed = 1.8
seconds = seconds * speed
amp(0.25 * math.abs(math.sin(seconds * 32)) * 0.5 + 0.5, 6)
amp(0.25 * math.abs(math.sin(seconds * 8)) * 0.5 + 0.5, 7)
amp(0.25 * math.abs(math.sin(seconds * 4)) * 0.5 + 0.5, 8)
if tt == 1 then
  note("C4", 6, 6)
  note("E4", 7, 7)
  note("G4", 8, 8)
elseif tt == 32 then
  note("C4", 6, 6)
  note("E5", 7, 7)
  note("G4", 8, 8)
elseif tt == 64 then
  note("C4", 6, 6)
  note("F5", 7, 7)
  note("G4", 8, 8)
elseif tt == 96 then
  note("C4", 6, 6)
  note("G5", 7, 7)
  note("A4", 8, 8)
end
