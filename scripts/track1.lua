amp(0, 4)
amp(0, 5)
amp(0, 6)
amp(0, 7)
amp(0, 8)

wave(2, 1)
amp(math.abs(math.sin(seconds * 32)) * 0.5 + 0.5, 1)
amp(math.abs(math.sin(seconds * 8)) * 0.5 + 0.5, 3)

local sequence = {}

sequence[1] = function()
  note("C4", 1)
  note("E4", 2)
  note("G4", 3)
end

sequence[2] = function()
  note("C4", 1)
  note("E5", 2)
  note("G4", 3)
end

sequence[3] = function()
  note("C4", 1)
  note("F5", 2)
  note("G4", 3)
end

sequence[4] = function()
  note("C4", 1)
  note("G5", 2)
  note("A4", 3)
end

sequence[5] = sequence[1]
sequence[6] = sequence[2]
sequence[7] = sequence[3]
sequence[8] = sequence[4]

sequence[9] = function()
    note("C3", 1)
    note("E3", 2)
    note("G3", 3)
end

sequence[10] = function()
    note("C3", 1)
    note("F4", 2)
    note("G3", 3)
end

sequence[11] = function()
    note("C3", 1)
    note("G4", 2)
    note("E4", 3)
end

sequence[12] = function()
    note("C3", 1)
    note("F4", 2)
    note("G3", 3)
end

local pos = math.floor(seconds % #sequence) + 1
print("pos: " .. pos)
sequence[pos]()
