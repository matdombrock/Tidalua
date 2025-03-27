
enable(2)
enable(3)
enable(4)

-- Super Saw
wave(3, 1)
wave(3, 2)
wave(3, 3)
wave(3, 4)

local function unison(note_num, detune_amt, start_osc, end_osc)
  local osc_count = end_osc - start_osc + 1
  local detune_each = detune_amt / osc_count
  for i = 1, osc_count do
    note(note_num, i)
    detune(detune_each * i, i)
  end
end

local note_num = 42 + (math.floor(tick / 128) * 5) % 42
local detune_amt = 0.01 + math.sin(seconds / 4) * 0.01
unison(note_num, detune_amt, 1, 4)
