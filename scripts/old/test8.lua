mono()

local sequence = {}

sequence[1] = function()
  note("C4", 0)
end

sequence[2] = function()
  note("D4", 0)
end

sequence[3] = function()
  note("E4", 0)
end

local speed = 2
local pos = seconds
pos = pos * speed
pos = math.floor(pos)
pos = pos % 3
pos = pos + 1

sequence[pos]()
