enable(2)
wave(3, 1)
wave(3, 2)
local lfo_pan = math.sin(tick / 256)
pan(lfo_pan, 1)
pan(-lfo_pan, 2)
local lfox = 0.5 + math.sin(tick / 1024) * 0.5
local lfo_lp1 = 1 - math.abs(math.sin(tick / 128 + (lfox * 64)))
local lfo_lp2 = 1 - math.abs(math.sin(tick / 16 + (lfox * 64)))
lowpass(1800 + (lfo_lp1 * 1500), 3, 1)
lowpass(1800 + (lfo_lp2 * 1500), 3, 2)

local lfo_detune1 = 1 - math.abs(math.sin(tick / 128))
local lfo_detune2 = 1 - math.abs(math.sin(tick / 64))
detune(lfo_detune1 * 0.01, 1)
detune(lfo_detune2 * 0.01, 2)

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
  note(rand_note() .. "4", 2)
end
local lfo_rng = 0.5 * math.sin(tick / 2048) + 0.5
lfo_rng = lfo_rng * 0.5
if tick % 128 == 0 then
  if math.random() > 0.5 then note(rand_note() .. "5", 2) end
end
if tick % (256 + 64) == 0 then
  if math.random() > 0.5 + lfo_rng then note(rand_note() .. "5", 2) end
end
if tick % (256 + 128 + 64) == 0 then
  if math.random() > 0.8 + lfo_rng then note(rand_note() .. "6", 2) end
end
if tick % (256 + 128 + 64) == 0 then
  if math.random() > 0.5 + lfo_rng then note(rand_note() .. "4", 2) end
end
if tick % (512 + 64) == 0 then
  if math.random() > 0.5 + lfo_rng then note(rand_note() .. "5", 2) end
end

