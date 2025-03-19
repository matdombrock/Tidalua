-- local speed = math.abs(math.sin(seconds /8)) + 1
local speed = 1.8
seconds = seconds * speed

enable(2)
enable(3)
amp(math.abs(math.sin(seconds * 32)) * 0.5 + 0.5, 1)
amp(math.abs(math.sin(seconds * 8)) * 0.5 + 0.5, 3)

local chords = {}

chords[1] = function()
  wave(2, 1)
  wave(1, 2)
  wave(1, 3)
  note("C4", 1)
  note("E4", 2)
  note("G4", 3)
end

chords[2] = function()
  note("C4", 1)
  note("E5", 2)
  note("G4", 3)
end

chords[3] = function()
  note("C4", 1)
  note("F5", 2)
  note("G4", 3)
end

chords[4] = function()
  note("C4", 1)
  note("G5", 2)
  note("A4", 3)
end

chords[5] = chords[1]
chords[6] = chords[2]
chords[7] = chords[3]
chords[8] = chords[4]

chords[9] = function()
  wave(2, 1)
  wave(2, 2)
  wave(2, 3)
  note("C3", 1)
  note("E3", 2)
  note("G3", 3)
end

chords[10] = function()
  note("C3", 1)
  note("F4", 2)
  note("G3", 3)
end

chords[11] = function()
    note("C3", 1)
    note("G4", 2)
    note("E4", 3)
end

chords[12] = function()
    note("C3", 1)
    note("F4", 2)
    note("G3", 3)
end

local chords_pos = math.floor(seconds % #chords) + 1
print("chords_pos: " .. chords_pos)
chords[chords_pos]()


enable(4)

local lead = {}
lead[1] = function()
  note("C6", 4)
end
lead[2] = function()
  note("E6", 4)
end
lead[3] = function()
  note("G6", 4)
end
lead[4] = function()
  note("A6", 4)
end
lead[5] = lead[1]
lead[6] = lead[2]
lead[7] = lead[3]
lead[8] = lead[4]
lead[9] = function()
  note("C5", 4)
end
lead[10] = function()
  note("E5", 4)
end
lead[11] = function()
  note("G5", 4)
end
lead[12] = function()
  note("A5", 4)
end
lead[13] = lead[1]
lead[14] = lead[2]
lead[15] = lead[3]
lead[16] = lead[4]

-- Pan the lead
pan(math.sin(seconds * 2), 4)
local lead_pos = math.floor((seconds * 2) % #lead) + 1
-- Add some vibrato to the lead
detune(math.sin(seconds * 32) * 0.025, 4)
if lead_pos > 12 and chords_pos > 8 then
  detune(math.sin(seconds * 32) * (math.sin(seconds / 32)), 4)
end
print("lead_pos: " .. lead_pos)
lead[lead_pos]()

enable(5)
wave(5, 5)
-- amp(0, 5)
local perc = {}
perc[1] = function()
  amp(1, 5)
  note("C2", 5)
end
perc[2] = function()
  amp(0, 5)
  note("E2", 5)
end
perc[3] = function()
  amp(1, 5)
  note("G2", 5)
end
perc[4] = function()
  amp(0, 5)
  note("A2", 5)
end
if chords_pos > 8 and seconds % 16 > 8 then
  local perc_pos = math.floor((seconds * 16) % #perc) + 1
  print("perc_pos: " .. perc_pos)
  perc[perc_pos]()
elseif seconds % 16 > 8 then
  amp(0, 5)
else
  amp(chords_pos / 8, 5)
end

