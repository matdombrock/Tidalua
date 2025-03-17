
mono()
local length = 3
local speed = 2
local function at_pos(check)
  return math.floor(seconds * speed) % length == check
end

if at_pos(0) then
  note("C4", 0)
elseif at_pos(1) then
  note("D4", 0)
elseif at_pos(2) then
  note("E4", 0)
end

