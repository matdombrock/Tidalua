local beat_ticks = 64
local beat_max = 16

local function beats()
  local beat = (tick / beat_ticks)
  if math.floor(tick / beat_ticks) ~= beat then
    return -1 -- Return -1 if we are not on a beat
  end
  beat = beat + 1
  beat = beat % beat_max
  return beat
end

enable(1)
enable(2)
enable(3)
local lfo1 = math.sin(tick / 512) * 0.5 + 0.5
local atk = 128
local sus = 64 + (lfo1 * 64)
local rel = 128
env(atk,sus,rel, 1)
env(atk,sus,rel, 2)
env(atk,sus,rel, 3)
-- Chords
if beats() == 1 then
  note("C3", 1)
  note("E3", 2)
  note("G3", 3)
end
if beats() == 5 then
  note("C4", 1)
  note("E4", 2)
  note("G4", 3)
end
if beats() == 7 then
  note("C4", 1)
  note("F4", 2)
  note("A4", 3)
end
if beats() == 9 then
  note("C4", 1)
  note("E4", 2)
  note("G4", 3)
end
if beats() == 11 then
  note("C4", 1)
  local posible = { "D4", "E4", "G4" }
  note(posible[math.random(1, #posible)], 2)
  note("F4", 3)
end

-- Melody
enable(4)
local lfo2 = math.sin(tick / 256) * 0.5 + 0.5
amp(lfo2, 4)
if beats() % 4 == 0 then
  note("C4", 4)
end
if beats() % 4 == 1 then
  note("C5", 4)
end
if beats() % 4 == 2 then
  note("G5", 4)
end

-- Melody 2
enable(5)
local lfo3 = math.sin(tick / 128) * 0.5 + 0.5
local lfo4 = math.sin(64 + tick / 4) * 0.5 + 0.5
freq(660 + lfo3 * 440, 5)
amp((lfo4 * lfo3) / 4, 5)

