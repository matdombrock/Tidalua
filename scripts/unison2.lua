enable(2)
enable(3)
enable(4)
enable(5)
enable(6)
enable(7)
enable(8)

-- Super (DUPER) Saw
wave(3, 1)
wave(3, 2)
wave(3, 3)
wave(3, 4)
wave(3, 5)
wave(3, 6)
wave(3, 7)
wave(3, 8)

local function unison(note_num, detune_amt)
  local detune_each = detune_amt / 8
  for i = 1, 8 do
    if i > 6 then
      note_num = note_num + 12
    elseif i > 4 then
      note_num = note_num + 5
    end
    note_midi(note_num, i)
    detune(detune_each * i, i)
  end
end
local note_num = 42 + (math.floor(tick / 128) * 5) % 42

unison(note_num, 0.02 + math.sin(seconds / 4) * 0.02)
