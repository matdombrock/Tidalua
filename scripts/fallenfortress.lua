enable(1)
enable(2)

enable(3)
wave(2, 3)
amp(0.8, 3)

enable(4)
wave(3, 4)

bus_amp(0.9)

local atk = 64
local sus = 16
local dec = 8
env(atk, sus, dec, 1)
env(atk + 16, sus, dec + 16, 2)

local notes = { "C", "E", "F", "G", "A" }
local last_rand = ""
local function rand_note()
  local note = notes[math.random(1, #notes)]
  while note == last_rand do
    note = notes[math.random(1, #notes)]
  end
  last_rand = note
  return note
end


if tick % 256 == 0 then
  note(rand_note() .. "3", 1)
end
if tick % 256 == 0 then
  note(rand_note() .. "5", 2)
end

if tick % 256 == 0 then
  note(rand_note() .. "2", 3)
end

if tick % 128 == 0 then
  note(rand_note() .. "4", 4)
end
local lfo_lead = math.sin(tick / 128) * 0.5 + 0.5
local lfo_lead2 = math.sin(tick / 1024) * 0.5 + 0.5
lowpass(2000 + (lfo_lead * 1500), 1, 4)
highpass(200 + (lfo_lead * 450), 1, 4)
amp(lfo_lead2, 4)

local lfo_bus = math.sin(512 + tick / 2048) * 0.5 + 0.5
bus_lowpass(1000 + (lfo_bus * 500), 1)
